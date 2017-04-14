#ifndef TOM_H
#define TOM_H

#include <QString>
#include <QFile>
#include <QDebug>
#include <QList>
#include <QPair>

#include <iostream>
#include <boost/algorithm/string.hpp>

class Tom
{
private:
    QFile* dictionaryFile;
    QList< QPair<QStringList, QStringList> > dictionaryL;

public:
    Tom(QString fileName);
    ~Tom();

    void talk();

private:
    QString tomResponse(const QString &question);
    QStringList AnsByRegex(const QString &word);
    bool shortenedOrExpanded(const QStringList &where, const QString &what,
                           const float &portion);

    bool addToDic(const QString &tomQue, const QString &userAns);

    bool isInList(const QStringList &list, const QString &word);
    //bool isVersionOf(const QString &first, const QString &second);

    void printDic();

    QString userResponse();
    QString getFileName();

    void readFile() throw (std::runtime_error);
    void saveFile() throw (std::runtime_error);
};

#endif // TOM_H
