/****************************************************************************
** Copyright (c) 2013-2014 Debao Zhang <hello@debao.me>
** All right reserved.
**
** Permission is hereby granted, free of charge, to any person obtaining
** a copy of this software and associated documentation files (the
** "Software"), to deal in the Software without restriction, including
** without limitation the rights to use, copy, modify, merge, publish,
** distribute, sublicense, and/or sell copies of the Software, and to
** permit persons to whom the Software is furnished to do so, subject to
** the following conditions:
**
** The above copyright notice and this permission notice shall be
** included in all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
** NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
** LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
** OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
** WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**
****************************************************************************/
#include "private/xlsxutility_p.h"
#include <QString>
#include <QtTest>
#include <QDateTime>

class UtilityTest : public QObject
{
    Q_OBJECT
    
public:
    UtilityTest();
    
private Q_SLOTS:
    void test_datetimeToNumber_data();
    void test_datetimeToNumber();

    void test_timeToNumber_data();
    void test_timeToNumber();

    void test_datetimeFromNumber_data();
    void test_datetimeFromNumber();

    void test_createSafeSheetName_data();
    void test_createSafeSheetName();
};

UtilityTest::UtilityTest()
{
}

void UtilityTest::test_datetimeToNumber_data()
{
    QTest::addColumn<QDateTime>("dt");
    QTest::addColumn<bool>("is1904");
    QTest::addColumn<double>("num");

    //Note, for number 0, Excel2007 shown as 1900-1-0, which should be 1899-12-31
    QTest::newRow("0") << QDateTime(QDate(1899, 12, 31), QTime(0,0)) << false << 0.0;
    QTest::newRow("0.0625") << QDateTime(QDate(1899, 12, 31), QTime(1,30)) << false << 0.0625;
    QTest::newRow("1.25") << QDateTime(QDate(1900, 1, 1), QTime(6, 0)) << false << 1.25;
    QTest::newRow("59") << QDateTime(QDate(1900, 2, 28), QTime(0, 0)) << false << 59.0;
    QTest::newRow("61") << QDateTime(QDate(1900, 3, 1), QTime(0, 0)) << false << 61.0;

    QTest::newRow("1904: 0") << QDateTime(QDate(1904, 1, 1), QTime(0,0)) << true << 0.0;
    QTest::newRow("1904: 1.25") << QDateTime(QDate(1904, 1, 2), QTime(6, 0)) << true << 1.25;
}

void UtilityTest::test_datetimeToNumber()
{
    QFETCH(QDateTime, dt);
    QFETCH(bool, is1904);
    QFETCH(double, num);

    QCOMPARE(QXlsx::datetimeToNumber(dt, is1904), num);
}

void UtilityTest::test_timeToNumber_data()
{
    QTest::addColumn<QTime>("t");
    QTest::addColumn<double>("num");

    QTest::newRow("0") << QTime(0,0) << 0.0;
    QTest::newRow("0.0625") << QTime(1, 30) << 0.0625;
    QTest::newRow("0.25") << QTime(6, 0) << 0.25;
    QTest::newRow("0.5") << QTime(12, 0) << 0.5;
}

void UtilityTest::test_timeToNumber()
{
    QFETCH(QTime, t);
    QFETCH(double, num);

    QCOMPARE(QXlsx::timeToNumber(t), num);
}

void UtilityTest::test_datetimeFromNumber_data()
{
    QTest::addColumn<QDateTime>("dt");
    QTest::addColumn<bool>("is1904");
    QTest::addColumn<double>("num");

    QTest::newRow("0") << QDateTime(QDate(1899, 12, 31), QTime(0,0)) << false << 0.0;
    QTest::newRow("0.0625") << QDateTime(QDate(1899, 12, 31), QTime(1,30)) << false << 0.0625;
    QTest::newRow("1.25") << QDateTime(QDate(1900, 1, 1), QTime(6, 0)) << false << 1.25;
    QTest::newRow("59") << QDateTime(QDate(1900, 2, 28), QTime(0,0)) << false << 59.0;
    QTest::newRow("61") << QDateTime(QDate(1900, 3, 1), QTime(0,0)) << false << 61.0;

    QTest::newRow("1904: 0") << QDateTime(QDate(1904, 1, 1), QTime(0,0)) << true << 0.0;
    QTest::newRow("1904: 1.25") << QDateTime(QDate(1904, 1, 2), QTime(6, 0)) << true << 1.25;
}

void UtilityTest::test_datetimeFromNumber()
{
    QFETCH(QDateTime, dt);
    QFETCH(bool, is1904);
    QFETCH(double, num);

    QCOMPARE(QXlsx::datetimeFromNumber(num, is1904), dt);
}

void UtilityTest::test_createSafeSheetName_data()
{
    QTest::addColumn<QString>("original");
    QTest::addColumn<QString>("result");

    QTest::newRow("[Hello]") << QString("[Hello]")<<QString("Hello");
    QTest::newRow("[Hello:Qt]") << QString("[Hello:Qt]")<<QString("Hello Qt");
    QTest::newRow("[Hello\\Qt/Xlsx]") << QString("[Hello\\Qt/Xlsx]")<<QString("Hello Qt Xlsx");
    QTest::newRow("[Hello\\Qt/Xlsx:Lib]") << QString("[Hello\\Qt/Xlsx:Lib]")<<QString("Hello Qt Xlsx Lib");
    QTest::newRow("'He'llo'") << QString("'He'llo'")<<QString("He'llo");
    QTest::newRow("'He'llo*Qt'") << QString("'He'llo*Qt'")<<QString("He'llo Qt");
    QTest::newRow("'He'llo*Qt?Lib'") << QString("'He'llo*Qt?Lib'")<<QString("He'llo Qt Lib");

    QTest::newRow(":'[Hello']") << QString(":'[Hello']")<<QString("Hello");
    QTest::newRow("':'[Hello']") << QString("':'[Hello']")<<QString("Hello");
    QTest::newRow("  ' : '[ Hello ' ]  ") << QString("  ' : '[ Hello ' ]  ")<<QString("Hello");
}

void UtilityTest::test_createSafeSheetName()
{
    QFETCH(QString, original);
    QFETCH(QString, result);

    QCOMPARE(QXlsx::createSafeSheetName(original), result);
}

QTEST_APPLESS_MAIN(UtilityTest)

#include "tst_utilitytest.moc"
