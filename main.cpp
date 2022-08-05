#include <QCoreApplication>
#include <QDir>
#include <QDebug>
#include <QRandomGenerator>
//=====================================================================================================
QStringList GetAllFiles(QString path,QString aliasPath="")
{
    QDir directory(path);
    int i=0;
    QStringList result;
    QStringList images = directory.entryList(QStringList() << "*.png" << "*.jpg" << "*.PNG"<< "*.bmp"<< "*.BMP" << "*.JPG",QDir::Files);
    foreach (QString item, images) {
        if(aliasPath=="")
        {
         result.append(path+"/"+item);
        }

        else {
            result.append(aliasPath+"/"+item);
        }
       }
    return  result;
}
//====================================================================================
void CreateListFile(QString path,QStringList files)
{
    int trainCount=files.count()*80;
    trainCount/=100;
    int testCount=files.count()*10;
    testCount/=100;
    int validationCount=files.count()-trainCount-testCount;
    qDebug()<<"train:"<<trainCount<<" validtion:"<<validationCount<<" test:"<<testCount;
    if(QFile::exists(path+"/train_list.txt"))
    {
      QFile::remove(path+"/train_list.txt") ;

    }
    if(QFile::exists(path+"/val_list.txt"))
    {
      QFile::remove(path+"/val_list.txt") ;

    }
    if(QFile::exists(path+"/test_list.txt"))
    {
      QFile::remove(path+"/test_list.txt") ;

    }
    QFile trainFile(path+"/train_list.txt");
QFile validationFile(path+"/val_list.txt");
QFile testFile(path+"/test_list.txt");

trainFile.open(QFile::ReadWrite|QFile::NewOnly);
testFile.open(QFile::ReadWrite|QFile::NewOnly);
validationFile.open(QFile::ReadWrite|QFile::NewOnly);
for(int i=0;i<trainCount;i++)
{
 trainFile.write(files[i].toLatin1()+"\n");

}
for(int i=0;i<validationCount;i++)
{
 validationFile.write(files[i+trainCount].toLatin1()+"\n");

}
for(int i=0;i<testCount;i++)
{
 testFile.write(files[i+trainCount+validationCount].toLatin1()+"\n");

}


trainFile.close();
testFile.close();
validationFile.close();
qDebug()<<"done";
}
QStringList ShuffleFileList(QStringList files)
{
  QStringList result;
  while (files.length()>0) {
      int randomIndex=QRandomGenerator::global()->bounded(files.length());
      result.append(files[randomIndex]);
      files.removeAt(randomIndex);

  }


  return result;
}
//=====================================================================================================
int main(int argc, char *argv[])
{



    QCoreApplication a(argc, argv);
    if(argc<3)
    {

        qDebug()<<"help:\n"
                  "creates test train and validation \n"
                  "./MakeDataset.sh [path] [output path] [prefix image path]\n"
                  "example ./MakeDataset.sh /home/user/CustomDataset/image /home/user/CustomDataset/ /mounted/CustomDataset/image\n";
        return 0;
    }
qDebug()<<"input folder files:"<<argv[1]<<" output path:"<<argv[2]<<" address prefix:"<<argv[3];
QStringList files=GetAllFiles(argv[1],argv[3]);

qDebug()<<"scanning complete:"<<files.count()<<" files";
CreateListFile(argv[2],ShuffleFileList(files));
return 0;

}
//=====================================================================================================
