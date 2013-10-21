/****************************************************************************
** Copyright (c) 2013 Debao Zhang <hello@debao.me>
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
#ifndef XLSXSTYLES_H
#define XLSXSTYLES_H

#include "xlsxglobal.h"
#include <QSharedPointer>
#include <QHash>
#include <QList>
#include <QStringList>

class QIODevice;

namespace QXlsx {

class Format;
struct NumberData;
struct FontData;
struct FillData;
struct BorderData;
class XmlStreamWriter;
class XmlStreamReader;

class XLSX_AUTOTEST_EXPORT Styles
{
public:
    Styles(bool createEmpty=false);
    ~Styles();
    Format *createFormat();
    void addFormat(Format *format);
    Format *xfFormat(int idx) const;

    QByteArray saveToXmlData();
    void saveToXmlFile(QIODevice *device);
    bool loadFromXmlFile(QIODevice *device);
    bool loadFromXmlData(const QByteArray &data);

private:
    friend class Format;

    void writeNumFmts(XmlStreamWriter &writer);
    void writeFonts(XmlStreamWriter &writer);
    void writeFills(XmlStreamWriter &writer);
    void writeFill(XmlStreamWriter &writer, FillData *fill);
    void writeBorders(XmlStreamWriter &writer);
    void writeSubBorder(XmlStreamWriter &writer, const QString &type, int style, const QColor &color);
    void writeCellXfs(XmlStreamWriter &writer);
    void writeDxfs(XmlStreamWriter &writer);

    bool readNumFmts(XmlStreamReader &reader);
    bool readFonts(XmlStreamReader &reader);
    bool readFills(XmlStreamReader &reader);
    bool readBorders(XmlStreamReader &reader);
    bool readCellXfs(XmlStreamReader &reader);

    QHash<QString, int> m_builtinNumFmtsHash;
    QList<QSharedPointer<NumberData> > m_customNumFmts;
    QHash<QString, QSharedPointer<NumberData> > m_customNumFmtsHash;
    QList<QSharedPointer<FontData> > m_fontsList; //Keep a copy of unique fonts
    QList<QSharedPointer<FillData> > m_fillsList; //Keep a copy of unique fills
    QList<QSharedPointer<BorderData> > m_bordersList; //Keep a copy of unique borders
    QHash<QByteArray, QSharedPointer<FontData> > m_fontsHash;
    QHash<QByteArray, QSharedPointer<FillData> > m_fillsHash;
    QHash<QByteArray, QSharedPointer<BorderData> > m_bordersHash;

    QList<QSharedPointer<Format> > m_createdFormatsList; //All created formats

    QList<Format *> m_xf_formatsList;
    QHash<QByteArray, Format *> m_xf_formatsHash;

    QList<Format *> m_dxf_formatsList;
    QHash<QByteArray, Format *> m_dxf_formatsHash;
};

}
#endif // XLSXSTYLES_H
