#include "spinboxdelegate.h"

SpinBoxDelegate::SpinBoxDelegate(QObject *parent) : QItemDelegate(parent) {

}

SpinBoxDelegate::~SpinBoxDelegate() {

}

QWidget* SpinBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {

    QSpinBox *editor = new QSpinBox(parent);
    editor->setMinimum(0);
    editor->setMaximum(100);
    return editor;
}


void SpinBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    // Get the value via index of the Model
    int value = index.model()->data(index, Qt::EditRole).toInt();

    // Put the value into the SpinBox
    QSpinBox *spinbox = static_cast<QSpinBox*>(editor);
    spinbox->setValue(value);
}


void SpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QSpinBox* spinBox = static_cast<QSpinBox*>(editor);
    spinBox->interpretText();
    int value = spinBox->value();
    model->setData(index, value, Qt::EditRole);
}

// Give the SpinBox the info on size and location
void SpinBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}