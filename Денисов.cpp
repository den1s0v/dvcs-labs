#include "createproductiondialog.h"

CreateProductionDialog::CreateProductionDialog(int userId, QWidget *parent) : QDialog(parent) {
	mUserId = userId;
	codec = QTextCodec::codecForLocale();
	ui.setupUi(this);
	setModal(true);

	QComboBox* comboBox = setComboBox();
	QSpinBox* spinBox = setSpinBox();
	ui.gridLayout->addWidget(comboBox, 1, 0);
	ui.gridLayout->addWidget(spinBox, 1, 1);
	connect(ui.cancelButton, SIGNAL(clicked()), this, SLOT(cancelCreate()));
	connect(ui.addWorkButton, SIGNAL(clicked()), this, SLOT(addWork()));
	connect(ui.createWorkButton, SIGNAL(clicked()), this, SLOT(createWork()));
}

CreateProductionDialog::~CreateProductionDialog() {

}

void CreateProductionDialog::cancelCreate() {
	reject();
}

void CreateProductionDialog::addWork() {
	int row = ui.gridLayout->rowCount();
	QComboBox* comboBox = setComboBox();
	QSpinBox* spinBox = setSpinBox();
	QPushButton* deleteButton = setDeleteButton(row);
	ui.gridLayout->addWidget(comboBox, row, 0);
	ui.gridLayout->addWidget(spinBox, row, 1);
	ui.gridLayout->addWidget(deleteButton, row, 2);
}

void CreateProductionDialog::deleteWork() {
	QPushButton* button = qobject_cast<QPushButton*>(sender());
	int row = button->property("row").toInt();
	for (int i = 0; i < 3; ++i) {
		QLayoutItem* layoutItem = ui.gridLayout->itemAtPosition(row, i);
		ui.gridLayout->removeItem(layoutItem);
		delete layoutItem->widget();
		delete layoutItem;
	}
	ui.gridLayout->update();
}

QPushButton* CreateProductionDialog::setDeleteButton(int row) {
	QPushButton* button = new QPushButton(codec->toUnicode("Убрать"), this);
	button->setStyleSheet("background:none;color:blue;text-decoration:underline;border:none;text-align:left;");
	button->setCursor(Qt::PointingHandCursor);
	button->setProperty("row", row);
	connect(button, SIGNAL(clicked()), this, SLOT(deleteWork()));
	return button;
}

QComboBox* CreateProductionDialog::setComboBox() {
	QComboBox* comboBox = new QComboBox(this);

	QSqlQuery query = DataBaseProvider::getQuery();
    query.prepare("SELECT * FROM type_product");
	bool success = DataBaseProvider::execQuery(query);

	//TODO
	//запрос к БД получение списка всех типов прдукции - Получение данных для comboBox
	if (success) {
		while (query.next()) {
			comboBox->addItem(query.value("name").toString(), query.value("id"));
		}
		comboBox->setCurrentIndex(0);
	}

	return comboBox;
}

/** This is my first commit to `master` branch
	An informative documenting comment for setSpinBox() method.
*/
QSpinBox* CreateProductionDialog::setSpinBox() {
	QSpinBox* spinBox = new QSpinBox(this);
	spinBox->setMinimum(1);
	spinBox->setMaximum(10000);
	return spinBox;
}

void CreateProductionDialog::createWork() {
	QString date = ui.workDateEdit->date().toString("dd.MM.yyyy");

	QList<QPair<int, QPair<QString, int> > > workList = QList<QPair<int, QPair<QString, int> > >();

	int countRow = ui.gridLayout->rowCount();
	QVariantList prodTypes;
	QVariantList numProduct;
	QVariantList statuses;
	QVariantList salary;

	QSqlQuery query = DataBaseProvider::getQuery();
	query.prepare("SELECT salary.id FROM salary WHERE salary.idWorker = ? AND salary.idBillingPeriod = (SELECT billng_period.id FROM billng_period WHERE endDate IS NULL OR endDate = '')");
	query.addBindValue(mUserId);
	bool success = DataBaseProvider::execQuery(query);
	query.next();
	int salaryId = query.value(0).toInt(); 

	for (int i = 1; i < countRow; ++i) {
		QPair<int, QPair<QString, int> > work;
		work.first = 0; /*ID заинсерченой записи*/
		if (ui.gridLayout->itemAtPosition(i, 0) == 0) {
			continue;
		}
		QComboBox* comboBox = qobject_cast<QComboBox*>(ui.gridLayout->itemAtPosition(i, 0)->widget());
		QSpinBox* spinBox = qobject_cast<QSpinBox*>(ui.gridLayout->itemAtPosition(i, 1)->widget());
		work.second = QPair<QString, int>(comboBox->currentText(), spinBox->value());
		prodTypes << comboBox->currentData().toInt();
		numProduct << spinBox->value();
		statuses << 0;
		salary << salaryId;
		workList.append(work);
	}
	
	int l = workList.length();
	for(int i = 0; i < l; i++) {
		QSqlQuery queryInsert =  DataBaseProvider::getQuery();
		queryInsert.prepare("INSERT INTO `fact_of_work`(`idType`, `numProduct`, `status`, `idSalary`) VALUES (?,?,?,?)");
		queryInsert.addBindValue(prodTypes.at(i).toInt());
		queryInsert.addBindValue(numProduct.at(i).toInt());
		queryInsert.addBindValue(statuses.at(i).toInt());
		queryInsert.addBindValue(salary.at(i).toInt());
		DataBaseProvider::execQuery(queryInsert);
		workList[i].first = query.lastInsertId().toInt();
	}

	emit worksCreated(date, workList);
}