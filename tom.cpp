#include "tom.h"

Tom::Tom(QString fileName)
{
    try{
        dictionaryFile = new QFile(fileName);
        readFile();
    } catch (std::runtime_error &ex){
        qWarning() << "Exception in Tom::Tom() : " << ex.what();
    }
}

Tom::~Tom()
{
    delete dictionaryFile;
}

void Tom::talk()
{
    try{
        printDic();

        QString userR = "ahoj"; //initial condition
        QString tomR;

        while(userR != "bye"){
            tomR = tomResponse(userR);
            std::cout << "Tom: " + tomR.toStdString()
                      << std::endl << std::endl;

            std::cout << "Ty: ";
            userR = userResponse();

            addToDic(tomR, userR);
        }
        saveFile();
    } catch (std::runtime_error ex){
        std::cerr << "Exception in talk(): " << ex.what() << std::endl;
    }
}

QString Tom::tomResponse(const QString &question)
{
    if(question == "") return "?";

    QStringList ansList;

    for(auto pair : dictionaryL){   //first look for exact match
        if(pair.first.indexOf(question) > -1){
            ansList = pair.second;
            break;
        }
    }

    if(ansList.empty()){
        ansList = AnsByRegex(question); //then for SHORTER OR LONGER
    }
                                //randomize answer
    if (!ansList.empty()) return ansList.at(qrand() % ansList.size());
    return question + "?";      //or return que?
}

QStringList Tom::AnsByRegex(const QString &word)
{
    QStringList ansList;

    if(word.length() > 8){
        for(auto pair : dictionaryL){       //check shorter forms of string
            if(shortenedOrExpanded(pair.first, word, (float)0.66)){
                ansList = pair.second;
                break;
            }
        }
    }

    else if(word.length() > 3){  //dont shorten shorter than 4
        for(auto pair : dictionaryL){       //check for even shorter forms
            if(shortenedOrExpanded(pair.first, word, (float)0.5)){
                ansList = pair.second;
                break;
            }
        }
    }

    return ansList;
}

/* the logic is to find a version of a word
 * improvised for slavic languages
 * shortened if with prefix and postfix "vy-MYSL-ene"
 * expanded if without prefix and postfix "VY-MYSL-ENE"
 */
bool Tom::shortenedOrExpanded(const QStringList &where, const QString &what,
                            const float &portion)
{
    QString word = what;
    while(word.length() > what.length()*portion){
        word.chop(1);   //shorten and check for possibly expanded on both sides
        QRegExp rx(".{0,15}" + word + ".{0,15}");
        if (where.indexOf(rx) > -1) return true;    //return word;
        QString word2 = word;                      //if you want the matched word

        while (word2.length() > what.length()*portion) {
            word2.remove(0, 1);
            QRegExp rx(".{0,15}" + word2 + ".{0,15}");
            if (where.indexOf(rx) > -1) return true; //return word2;
        }
    }
    return false;
}

bool Tom::addToDic(const QString &tomQue, const QString &userAns)
{
    if(userAns == "bye") return false;
    if(userAns == "neviem" || userAns == "kasli na to") return false;
    if(tomQue == "" || *(tomQue.cend()-2) == ' ') return false;

    for(auto &pair : dictionaryL){
        if(pair.first.indexOf(tomQue) > -1){
            if(pair.second.indexOf(userAns) > -1){
                //que and ans already there -> do nothing
                return true;
            } else {
                //ans not found -> add it
                pair.second.append(userAns);
                return true;
            }
        }
    }

        //que not found -> add line
    QStringList questions(tomQue);
    QStringList answers(userAns);
    QPair<QStringList, QStringList> pair(questions, answers);
    dictionaryL.append(pair);

    return true;
}

bool Tom::isInList(const QStringList &list, const QString &word)
{
    return list.indexOf(word) > -1;
}

void Tom::printDic()
{
    std::cout << "Welcome to Talking Tom, type 'bye' to quit" << std::endl;
    std::cout << "This is Tom's dictionary:" << std::endl << std::endl;

    for(auto pair : dictionaryL){
        for(QString question : pair.first){
            if(&question != *(pair.first.cend()-1))
                std::cout << question.toStdString() << ", ";
            else std::cout << question.toStdString();
                    //leaves out the last comma
        }
        std::cout << " : ";
        for(QString answer : pair.second){
            if(&answer != *(pair.second.cend()-1))
                std::cout << answer.toStdString() << ", ";
            else std::cout << answer.toStdString();
                    //leaves out the last comma
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

QString Tom::userResponse()
{
    std::string input;
    std::getline(std::cin, input);
    boost::to_lower(input);
    return QString(input.c_str());
}

QString Tom::getFileName()
{
    return dictionaryFile->fileName();
}

void Tom::readFile() throw (std::runtime_error)
{
    if(dictionaryFile->open(QFile::ReadOnly)){

        while (!dictionaryFile->atEnd()) {
            QString line = QString(dictionaryFile->readLine());
            if(*(line.cend()-1) == '\n') line.chop(1);
            if(*(line.cend()-1) == '\r') line.chop(1);
            QStringList splitLine = line.split(" : ");
            QStringList queList = splitLine.at(0)
                    .split(", ", QString::SkipEmptyParts);

            QStringList ansList = splitLine.at(1)
                    .split(", ", QString::SkipEmptyParts);

            QPair<QStringList, QStringList> dicLine(queList, ansList);
            dictionaryL.append(dicLine);
        }
    dictionaryFile->close();

    } else throw std::runtime_error("Exception in readFile(). "
                                    "File not open to ReadOnly");
}

void Tom::saveFile() throw (std::runtime_error)
{
    QTextStream stream(dictionaryFile);

    if(dictionaryFile->open(QFile::WriteOnly)){

        for(auto pair : dictionaryL){
            for(auto question : pair.first){
                if(&question != *(pair.first.cend()-1))
                    stream << question << ", ";
                else stream << question;    //leaves out the last comma
            }
            stream << " : ";
            for(auto answer : pair.second){
                if(&answer != *(pair.second.cend()-1))
                    stream << answer << ", ";
                else stream << answer;    //leaves out the last comma
            }
            stream << "\r\n";
        }
        stream.flush();
        dictionaryFile->close();

    } else throw std::runtime_error("Exception in readFile(). "
                                    "File not open to WriteOnly");
}

