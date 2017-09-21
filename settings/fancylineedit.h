/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
****************************************************************************/

#ifndef FANCYLINEEDIT_H_
#define FANCYLINEEDIT_H_


#include <QAbstractButton>

#include <functional>
#include <QLineEdit>

QT_BEGIN_NAMESPACE
class QEvent;
QT_END_NAMESPACE

//namespace Utils {

class FancyLineEditPrivate;

class IconButton: public QAbstractButton
{
    Q_OBJECT
    Q_PROPERTY(float iconOpacity READ iconOpacity WRITE setIconOpacity)
    Q_PROPERTY(bool autoHide READ hasAutoHide WRITE setAutoHide)
    Q_PROPERTY(QPixmap pixmap READ pixmap WRITE setPixmap)
public:
    explicit IconButton(QWidget *parent = 0);
    void paintEvent(QPaintEvent *event);
    void setPixmap(const QPixmap &pixmap) { m_pixmap = pixmap; update(); }
    QPixmap pixmap() const { return m_pixmap; }
    float iconOpacity() { return m_iconOpacity; }
    void setIconOpacity(float value) { m_iconOpacity = value; update(); }
    void animateShow(bool visible);

    void setAutoHide(bool hide) { m_autoHide = hide; }
    bool hasAutoHide() const { return m_autoHide; }

    QSize sizeHint() const;

protected:
    void keyPressEvent(QKeyEvent *ke);
    void keyReleaseEvent(QKeyEvent *ke);

private:
    float m_iconOpacity;
    bool m_autoHide;
    QPixmap m_pixmap;
};

class FancyLineEdit : public QLineEdit
{
    Q_OBJECT
    Q_ENUMS(Side)

    // Validation.
    Q_PROPERTY(QString initialText READ initialText WRITE setInitialText DESIGNABLE true)
    Q_PROPERTY(QColor errorColor READ errorColor WRITE setErrorColor DESIGNABLE true)
    Q_PROPERTY(QColor okColor READ okColor WRITE setOkColor DESIGNABLE true)

public:
    enum Side {Left = 0, Right = 1};

    explicit FancyLineEdit(QWidget *parent = 0);
    ~FancyLineEdit();

    QPixmap buttonPixmap(Side side) const;
    void setButtonPixmap(Side side, const QPixmap &pixmap);

    QMenu *buttonMenu(Side side) const;
    void setButtonMenu(Side side, QMenu *menu);

    void setButtonVisible(Side side, bool visible);
    bool isButtonVisible(Side side) const;
    QAbstractButton *button(Side side) const;

    void setButtonToolTip(Side side, const QString &);
    void setButtonFocusPolicy(Side side, Qt::FocusPolicy policy);

    // Set whether tabbing in will trigger the menu.
    void setMenuTabFocusTrigger(Side side, bool v);
    bool hasMenuTabFocusTrigger(Side side) const;

    // Set if icon should be hidden when text is empty
    void setAutoHideButton(Side side, bool h);
    bool hasAutoHideButton(Side side) const;


    // Filtering

    // Enables filtering
    void setFiltering(bool on);


    //  Validation

    // line edit, (out)errorMessage -> valid?
//    typedef std::function<bool(FancyLineEdit *, QString *)> ValidationFunction;
    enum State { Invalid, DisplayingInitialText, Valid };

    State state() const;
    bool isValid() const;
    QString errorMessage() const;

    QString initialText() const;
    void setInitialText(const QString &);

    QColor errorColor() const;
    void setErrorColor(const  QColor &c);

    QColor okColor() const;
    void setOkColor(const  QColor &c);

//    void setValidationFunction(const ValidationFunction &fn);
//    static ValidationFunction defaultValidationFunction();
    void onEditingFinished();
public slots:
    void validate();

protected:
    // Custom behaviour can be added here.
    virtual void handleChanged(const QString &) {}

signals:
    void buttonClicked(/*Utils::*/FancyLineEdit::Side side);
    void leftButtonClicked();
    void rightButtonClicked();

    void filterChanged(const QString &);

    void validChanged(bool validState);
    void validReturnPressed();

protected:
    void resizeEvent(QResizeEvent *e);

    virtual QString fixInputString(const QString &string);

private slots:
    void iconClicked();
private:
    static bool validateWithValidator(FancyLineEdit *edit, QString *errorMessage);

    void updateMargins();
    void updateButtonPositions();
    friend class FancyLineEditPrivate;

    FancyLineEditPrivate *d;
};

//} // namespace Utils
#endif
