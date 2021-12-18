#include "ConvertInfo.h"
#include <QDir>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QDebug>
#include <QProcess>


void Video_Convert::init()
{
    m_floor = 0;
    m_convertList.clear();
    m_outDir.clear();
}

QString Video_Convert::validateTitle(QString title)
{
    return  title.replace(QRegExp("[\\\\:*?\"<>|& #]"), "_");
}

QString Video_Convert::join(QString path1, QString path2)
{
    QString strJoin = path1.append("//" + path2);

    return  strJoin;
}

void Video_Convert::clearDir(QString path)
{
    if(path.isEmpty())
    {
        return;
    }
    QDir dir(path);

    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
    foreach(QFileInfo fileInfo, dir.entryInfoList())
    {
        if(fileInfo.isFile())
        {
            if(!fileInfo.isWritable())
            {
                QFile file(fileInfo.absoluteFilePath());
                file.setPermissions(QFile::WriteOwner);
            }

            fileInfo.dir().remove(fileInfo.fileName());

        }
        else if(fileInfo.isDir())
        {
            clearDir(fileInfo.absoluteFilePath());
        }
    }
    dir.rmpath(dir.absolutePath());
}

void Video_Convert::traversalSource(QString path)
{

    if(path.isEmpty())
    {
        return;
    }
    QDir dir(path);

    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
    foreach(QFileInfo f, dir.entryInfoList())
    {
        if(f.isDir())
        {
            if(f.fileName()=="." || f.fileName() =="..")
                continue;
            qDebug()<<"Entry Dir"<<f.absoluteFilePath();
            traversalSource(f.absoluteFilePath());

        }
        else if(f.isFile())
        {
            QString filePath = f.absoluteFilePath();

            if (m_floor == m_convertList.length()) {
                // 解析json字符串
                if (filePath.contains("entry.json") || filePath.contains("index.json")) {
                    parseJson(filePath, stConvertInfo);
                    if (filePath.contains("entry.json")) {
                        qDebug()<<stConvertInfo._title + " " + stConvertInfo._part;
                    }
                } else if (filePath.contains("video.m4s")) {
                    stConvertInfo._video = filePath.replace("\\", "/");
                    stConvertInfo._type = "m4s";
                    qDebug()<<stConvertInfo._video;
                } else if(filePath.contains("audio.m4s")) {
                    stConvertInfo._audio = filePath.replace("\\", "/");;
                    qDebug()<<stConvertInfo._audio;
                } else if (filePath.endsWith(".blv")) {
                    stConvertInfo._blv.append(filePath.replace("\\", "/"));\
                    stConvertInfo._type = "blv";
                } else if (filePath.endsWith(".m3u8")) {
                    stConvertInfo._type = "m3u8";

                    /*# 寻找同级目录文件夹作为m3u8实际视频存放位置，跟该索引文件的层级关系为index
                    #                                                                  |
                    #                                                                folder->real m3u8 video
                    # 取得m3u8 index之前的路径
                    */
                    QString m3u8_folder = filePath.replace("\\", "/");
                    // # :不包括最后一个'/'位置，以得到文件父的路径
//                    m3u8_folder = m3u8_folder[0:m3u8_folder.rfind('/')]
//                    with open(filepath, 'r') as m3f:
//                        lineStrs = m3f.readlines()
//                        for lineStr in lineStrs:
//                            lineStr = lineStr.replace('\n', '')
//                            if lineStr.find('#EXT') == -1:
//                                splitStr = lineStr.split('/')
//                                # 例如：E:/Quark/Download/1/a (1,a为后补的两个字符串)
//                                convert_info['m3u8'].append(
//                                    m3u8_folder+'/'+splitStr[len(splitStr)-2]+'/'+splitStr[len(splitStr)-1])

//                        convert_info['title'] = os.path.splitext(
//                            filepath[filepath.rfind('\\')+1:len(filepath)])[0]  # 获取去除路径与后缀名以后的单独文件名
//                        convert_info['title'] = convert_info['title'].replace(
//                            ' ', '')
//                        if convert_info['title'] != '':
//                            allfile.append(convert_info)
//                            init_convert_info()
//                            floor += 1
                }
                if (stConvertInfo._type == "m4s" && !stConvertInfo._title.isEmpty() &&
                       !stConvertInfo._video.isEmpty() && !stConvertInfo._audio.isEmpty() ) {
                    m_convertList.append(stConvertInfo);
                    stConvertInfo.init();
                    m_floor++;
                } else if(stConvertInfo._type == "blv" && !stConvertInfo._title.isEmpty() &&
                          !stConvertInfo._part.isEmpty() && stConvertInfo._blv_num > 0 &&
                          stConvertInfo._blv.length() >= stConvertInfo._blv_num) {
                    m_convertList.append(stConvertInfo);
                    stConvertInfo.init();
                    m_floor++;
                }
            }
        }
    }
}

bool Video_Convert::parseJson(QString filePath, st_convert_info &stConvertInfo)
{
    QFile file(filePath);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString value = file.readAll();

    QJsonParseError parseJsonErr;
    QJsonDocument document = QJsonDocument::fromJson(value.toUtf8(), &parseJsonErr);
    if (!(parseJsonErr.error == QJsonParseError::NoError)) {
        return false;
    }
    QJsonObject jsonObject = document.object();
    if (filePath.contains("entry.json")) {
        QString title = jsonObject["title"].toString();
        stConvertInfo._title = title;
        if (jsonObject.contains((QStringLiteral("page_data")))) {
            QJsonValue jsonValueList = jsonObject.value(QStringLiteral("page_data"));
            QJsonObject item = jsonValueList.toObject();
            if (item.contains(QStringLiteral("part"))) {
                stConvertInfo._part = item["part"].toString();
            } else {
                stConvertInfo._part = title;
            }
        } else {
            QJsonValue jsonValueList = jsonObject.value(QStringLiteral("ep"));
            QJsonObject item = jsonValueList.toObject();
            stConvertInfo._part = item["index"].toString();
        }
       stConvertInfo._part = validateTitle(stConvertInfo._part);
    } else if(filePath.contains("index.json")) {
        if (jsonObject.contains((QStringLiteral("segment_list")))) {
            QJsonValue arrayValue = jsonObject.value((QStringLiteral("segment_list")));
            if (arrayValue.isArray())
            {
                QJsonArray array = arrayValue.toArray();
                stConvertInfo._blv_num = array.size();
            }
        }
    }


    return  true;
}

void Video_Convert::run()
{
    int index = 0;
    QDir dir;
    QString outDir = m_outDir;
    QString mergeName = "mergeflv.txt";
    foreach(auto video_info, m_convertList) {
       if (video_info._part !="" && video_info._title != video_info._part)  {
           outDir +='/' + video_info._title;
       }
       outDir.replace(" ", "");
       if(!dir.exists(outDir)) {
           dir.mkdir(outDir);
       }
       if (video_info._part !="" && video_info._title != video_info._part)  {
            emit updateUI(1, video_info._title+'/'+video_info._part, index);
       } else {
           emit updateUI(1, video_info._title, index);
       }
       // 执行ffmpeg转码操作
       if (video_info._type == "m4s") {

           QString outFile;
           if (outDir.endsWith("/")) {
                 outFile = outDir + video_info._part;
            } else {
                 outFile = outDir + '/' + video_info._part;
            }
           qDebug()<<outFile+".mp4开始合并";
           QProcess* poc = new QProcess(this);
           QString command = QString("ffmpeg -i "+ video_info._video + " -i " + video_info._audio + " -codec copy " + outFile+".mp4");
           poc->setProcessChannelMode(QProcess::MergedChannels);
           poc->start(command);

           qDebug()<<outFile+".mp4合并完成";
       } else if(video_info._type == "blv") {
           QString outFile = outDir + '/' + video_info._part;
           qDebug()<<outFile+".mp4开始合并";
            if (video_info._blv_num == 1) // 单文件直接移动重命名
            {

            } else { // 多文件合并转换输出

                if (QFile::exists(mergeName)) {
                    QFile::remove(mergeName);
                    foreach(auto blv_file, video_info._blv) {
                        // 将文件重命名为flv文件
                        QString blv_file_no_ext = blv_file.remove(".blv");
                        QString blv_file_add_ext = blv_file_no_ext + ".flv";
                        // 重命名文件
                        QFile::rename(blv_file, blv_file_add_ext);
                        // 追加打开合并脚本,不存在则创建
                        QFile fileMerge(mergeName);
                        if (fileMerge.open(QIODevice::Append)) {
                            fileMerge.write("file ");
                            QString blv_file_write = "'"+blv_file_add_ext+"'"+"\n";
                            fileMerge.write(blv_file_write.toStdString().c_str());
                            fileMerge.close();
                        }
                    }
                    // 多个flv文件合并

                }
            }
           qDebug()<<outFile+".mp4合并完成";
       } else if (video_info._type == "m3u8") {

        } else { // 格式错误
           emit updateUI(4, video_info._type,index);
           return;
       }
       // 当前合并完成
       emit updateUI(2, video_info._title+"/"+video_info._part, index);
       index++;
    }
    // 删除中间件
    if (QFile::exists(mergeName)) {
         QFile::remove(mergeName);
    }
    emit updateUI(3, "转码工作全部完成", index);

}
