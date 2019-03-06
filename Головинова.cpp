#include "clientwindow.h"

#define ucode(str) codec->toUnicode(str)

ClientWindow::ClientWindow(QWidget *parent) : QMainWindow(parent) {
	/*QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName("serv30.hostland.ru");
	db.setPort(3306);
	db.setDatabaseName("host1261993_politeh");
	db.setUserName("host1261993_politeh");
	db.setPassword("34L82jwA");
	bool ok = db.open();
	db.lastError().text().toStdString();*/
	
	codec = QTextCodec::codecForLocale();
	spinBoxDelegate = new SpinBoxDelegate(this);

	//Стартовая настройка главного окна
	//Изменения для первого коммита в мастер
	/*
	ui.setupUi(this);
	ui.completedWorksTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.completedWorksTableWidget->setColumnHidden(0, true);
	*/
	ui.setupUi(this);
	ui.completedWorksTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.completedWorksTableWidget->setColumnHidden(0, true);
	ui.completedWorksTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.workersTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.workersTableWidget->setColumnHidden(0, true);
	ui.workersTableWidget->setColumnHidden(4, true);
	ui.workersTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.productionTypesTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.productionTypesTableWidget->setColumnHidden(0, true);
	ui.productionTypesTableWidget->setItemDelegateForColumn(2, spinBoxDelegate);
	ui.workersSalaryTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.workersSalaryTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.productionTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.productionTableWidget->setColumnHidden(0, true);
	ui.productionTableWidget->setColumnHidden(1, true);
	ui.productionTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.payrollHistoryTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.workHistoryTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	ui.hiddenIdWorkSpinBox->setVisible(false);
	ui.hiddenIdWorkerSpinBox->setVisible(false);
	
	ui.backStatisticButton->setVisible(false);
	ui.createStatisticButton->setVisible(false);
	ui.mySalaryAction->setVisible(false);
	ui.findWorkerLineEdit->setVisible(false);
	ui.findWorkerButton->setVisible(false);
	ui.showStatisticsButton->setVisible(false);
	ui.showWorkButton->setVisible(false);
	ui.setAdjustmentsButton->setVisible(false);

	//Коннекты главного окна
	connect(ui.myWorkAction, SIGNAL(triggered()), this, SLOT(showMyWorkPage()));
	connect(ui.mySalaryAction, SIGNAL(triggered()), this, SLOT(showMySalaryPage()));
	connect(ui.workersAction, SIGNAL(triggered()), this, SLOT(showWorkersPage()));
	connect(ui.productionTypesAction, SIGNAL(triggered()), this, SLOT(showProductionTypesPage()));
	connect(ui.completedWorksAction, SIGNAL(triggered()), this, SLOT(showCompletedWorksPage()));
	connect(ui.workerExitAction, SIGNAL(triggered()), this, SLOT(exitFromAccount()));
	connect(ui.headExitAction, SIGNAL(triggered()), this, SLOT(exitFromAccount()));
	
	/*connect(ui.showStatisticsButton, SIGNAL(clicked()), this, SLOT(showStartPageStatistic()));
	connect(ui.createSalaryStatisticButton, SIGNAL(clicked()), this, SLOT(showSettingsSalaryStatistic()));
	connect(ui.createWorkerStatisticButton, SIGNAL(clicked()), this, SLOT(showSettingsWorkerStatistic()));
	connect(ui.backStatisticButton, SIGNAL(clicked()), this, SLOT(showStartPageStatistic()));
	connect(ui.createStatisticButton, SIGNAL(clicked()), this, SLOT(showStatistic()));

	connect(ui.closeBillingPeriodButton, SIGNAL(clicked()), this, SLOT(closeBillingPeriod()));

	connect(ui.registerNewWorkerButton, SIGNAL(clicked()), this, SLOT(getRegisterWorkerDialog()));
	connect(ui.createNewProductionTypeButton, SIGNAL(clicked()), this, SLOT(getCreateProductionTypeDialog()));
	connect(ui.createNewRecordButton, SIGNAL(clicked()), this, SLOT(getCreateProductionDialog()));
	*/
	connect(ui.workersTableWidget, SIGNAL(itemSelectionChanged()), this, SLOT(showWorkerData()));
	connect(ui.editWorkerInfoButton, SIGNAL(clicked()), this, SLOT(showEditWorkerInfo()));
	connect(ui.saveWorkerInfoButton, SIGNAL(clicked()), this, SLOT(saveEditWorkerInfo()));

	connect(ui.reviewedWorksCheckBox, SIGNAL(stateChanged(int)), this, SLOT(showReviewedWorks(int)));
	connect(ui.notReviewedWorksCheckBox, SIGNAL(stateChanged(int)), this, SLOT(showNotReviewedWorks(int)));

	connect(ui.productionTableWidget, SIGNAL(itemSelectionChanged()), this, SLOT(showProductionData()));
	connect(ui.productonEditButton, SIGNAL(clicked()), this, SLOT(showEditWorkInfo()));
	connect(ui.productionSaveButton, SIGNAL(clicked()), this, SLOT(saveEditWorkInfo()));
	connect(ui.productionDeleteButton, SIGNAL(clicked()), this, SLOT(deleteWorkInfo()));
	
	//Стартовый запуск
	authorizationDialog = new AuthorizationDialog(this);
	connect(authorizationDialog, SIGNAL(showMainWindow(int, bool)), this, SLOT(showWindow(int, bool)));
	authorizationDialog->show();
}

ClientWindow::~ClientWindow() {

}

void ClientWindow::setVisibleStatisticButtons(bool isVisible) {
	ui.backStatisticButton->setVisible(isVisible);
	ui.createStatisticButton->setVisible(isVisible);
}

void ClientWindow::getAdminWindow() {
	ui.workerMenu->menuAction()->setVisible(false);
	ui.headMenu->menuAction()->setVisible(true);
	showWorkersPage();
}

void ClientWindow::getWorkerWindow() {
	ui.workerMenu->menuAction()->setVisible(true);
	ui.headMenu->menuAction()->setVisible(false);
	showMyWorkPage();
}

QToolButton* ClientWindow::getWorkControlButton(int workId, int row) {
	QToolButton* button = new QToolButton(ui.productionTypesTableWidget);
	QMenu* menu = new QMenu(button);
	QAction* acceptAction = menu->addAction(ucode("Принять"));
	acceptAction->setIcon(QPixmap("images/accept.png", "PNG"));
	acceptAction->setProperty("id", workId);
	connect(acceptAction, SIGNAL(triggered()), this, SLOT(acceptWork()));
	QAction* rejectAction = menu->addAction(ucode("Отклонить"));
	rejectAction->setIcon(QPixmap("images/reject.png", "PNG"));
	rejectAction->setProperty("id", workId);
	connect(rejectAction, SIGNAL(triggered()), this, SLOT(rejectWork()));
	button->setMenu(menu);
	button->setText(ucode("Управление записью"));
	button->setPopupMode(QToolButton::InstantPopup);
	button->setProperty("row", row);
	return button;
}

QWidget* ClientWindow::getTypesControlWidget(int typeId) {
	QWidget* widget = new QWidget();
	QPushButton* editButton = new QPushButton(ucode("Редактировать"), widget);
	connect(editButton, SIGNAL(clicked()), this, SLOT(editProductionType()));
	//QPushButton* deleteButton = new QPushButton(ucode("Удалить"), widget);
	//connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteProductionType()));
	QPushButton* saveButton = new QPushButton(ucode("Сохранить"), widget);
	saveButton->setVisible(false);
	connect(saveButton, SIGNAL(clicked()), this, SLOT(saveProductionType()));
	QHBoxLayout *layout = new QHBoxLayout;
	layout->addWidget(editButton);
	//layout->addWidget(deleteButton);
	layout->addWidget(saveButton);
	widget->setLayout(layout);
	widget->setProperty("id", typeId);
	return widget;
}

void ClientWindow::showWindow(int currentUser, bool isAdmin) {
	authorizationDialog->reject();
	currentUserId = currentUser;
	if (isAdmin) {
		getAdminWindow();
	} 
	else {
		getWorkerWindow();
	}

	setCurrentBillingPeriod();
	show();
}


void ClientWindow::setCurrentBillingPeriod() {
	QSqlQuery query = DataBaseProvider::getQuery();
    query.prepare("SELECT * FROM billng_period WHERE endDate IS NULL OR endDate = '';");
	bool success = DataBaseProvider::execQuery(query);
	query.next();
	ui.billingPeriodInfoWorkerDataLabel->setText(ucode("с ") + query.value("startDate").toString() + ucode(" г."));
	ui.billingPeriodInfoHeadDataLabel->setText(ucode("с ") + query.value("startDate").toString() + ucode(" г."));
}

void ClientWindow::exitFromAccount() {
	int answer = QMessageBox::question(
		this,
		ucode("Выход из аккаунта"),
		ucode("Вы действительно хотите выйти из системы?"),
		ucode("Да"),
		ucode("Нет")
		);
	if (answer == 0) {
		close();
		authorizationDialog = new AuthorizationDialog(this);
		connect(authorizationDialog, SIGNAL(showMainWindow(int, bool)), this, SLOT(showWindow(int, bool)));
		authorizationDialog->show();
	}
}

void ClientWindow::showMyWorkPage() {
	ui.mainWindowStackedWidget->setCurrentIndex(0);
	ui.workerWorkAreaStackedWidget->setCurrentIndex(0);
	ui.productionInfoWorkerStackedWidget->setCurrentIndex(0);
	ui.productionTableWidget->clearContents();

	QSqlQuery query = DataBaseProvider::getQuery();
    query.prepare("SELECT fact_of_work.id, type_product.name, fact_of_work.numProduct, fact_of_work.status FROM fact_of_work INNER JOIN type_product ON fact_of_work.idType = type_product.id INNER JOIN salary ON fact_of_work.idSalary = salary.id AND salary.idWorker = ? WHERE salary.idBillingPeriod = (SELECT id FROM billng_period WHERE billng_period.endDate IS NULL OR billng_period.endDate = '')");
	query.addBindValue(currentUserId);
	bool success = DataBaseProvider::execQuery(query);

	if (success) {
		int row = 0;
		ui.productionTableWidget->clearContents();
		ui.productionTableWidget->setRowCount(query.size());
		while (query.next()) {
			ui.productionTableWidget->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
			ui.productionTableWidget->setItem(row, 2, new QTableWidgetItem(query.value(1).toString()));
			ui.productionTableWidget->setItem(row, 3, new QTableWidgetItem(query.value(2).toString()));
			int status = query.value(3).toInt();
			QString statusString;
			switch (status) {
				case 0:
					statusString = ucode("Новая");
					break;
				case 1:
					statusString = ucode("Принято");
					break;
				case 2:
					statusString = ucode("Отклонено");
					break;
				case 3:
					statusString = ucode("Отредактировано");
					break;
				case 4:
					statusString = ucode("Удалено");
					break;
			}
			ui.productionTableWidget->setItem(row, 4, new QTableWidgetItem(statusString));
			row++;
		}
	}
}

void ClientWindow::showMySalaryPage() {
	ui.mainWindowStackedWidget->setCurrentIndex(0);
	ui.workerWorkAreaStackedWidget->setCurrentIndex(1);
	ui.workerSalaryInfoStackedWidget->setCurrentIndex(0);

	// Вывод истории зарплаты у рабочего
	//currentUserId;
	//ui.payrollHistoryTableWidget->
}

void ClientWindow::showWorkersPage() {
	ui.mainWindowStackedWidget->setCurrentIndex(1);
	ui.headWorkAreaStackedWidget->setCurrentIndex(0);
	ui.workerInfoStackedWidget->setCurrentIndex(0);

	QSqlQuery query = DataBaseProvider::getQuery();
	//TODO
	//Исправить запрос на вывод сотрудника, а не юзера
	//Сделать первоначальный селект при открытии страницы, и больше к БД не обращаться при перекликивании страниц
    query.prepare("SELECT t1.id, t1.hiringday, t1.idUser, t2.fio, t2.login FROM worker as t1 JOIN user as t2 ON t1.idUser = t2.id ");
	bool success = DataBaseProvider::execQuery(query);

	if(success){
		ui.workersTableWidget->clearContents();
		ui.workersTableWidget->setRowCount(query.size());
		int i=0;
		while (query.next()) {
			ui.workersTableWidget->setItem(i, 0, new QTableWidgetItem(query.value("id").toString()));
			ui.workersTableWidget->setItem(i, 1, new QTableWidgetItem(query.value("fio").toString()));
			ui.workersTableWidget->setItem(i, 2, new QTableWidgetItem(query.value("login").toString()));
			ui.workersTableWidget->setItem(i, 3, new QTableWidgetItem(ucode("В штате")));
			ui.workersTableWidget->setItem(i, 4, new QTableWidgetItem(query.value("hiringday").toString()));
			i++;
		}
	}
}

void ClientWindow::showProductionTypesPage() {
	ui.mainWindowStackedWidget->setCurrentIndex(1);
	ui.headWorkAreaStackedWidget->setCurrentIndex(1);

	QSqlQuery query = DataBaseProvider::getQuery();
	//TODO
	//Сделать первоначальный селект при открытии страницы, и больше к БД не обращаться при перекликивании страниц
    query.prepare("SELECT * FROM type_product");
	bool success = DataBaseProvider::execQuery(query);

	if (success) {
		ui.productionTypesTableWidget->clearContents();
		ui.productionTypesTableWidget->setRowCount(query.size());
		int i=0;
		while (query.next()) {
			ui.productionTypesTableWidget->setItem(i, 0, new QTableWidgetItem(query.value("id").toString()));
			ui.productionTypesTableWidget->setItem(i, 1, new QTableWidgetItem(query.value("name").toString()));
			ui.productionTypesTableWidget->item(i, 1)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
			ui.productionTypesTableWidget->setItem(i, 2, new QTableWidgetItem(query.value("cost").toString()));
			ui.productionTypesTableWidget->item(i, 2)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
			ui.productionTypesTableWidget->setCellWidget(i, 3, getTypesControlWidget(query.value("id").toInt()));
			ui.productionTypesTableWidget->setRowHeight(i, 50);
			i++;
		}
	}
}

void ClientWindow::showCompletedWorksPage() {
	ui.mainWindowStackedWidget->setCurrentIndex(1);
	ui.headWorkAreaStackedWidget->setCurrentIndex(2);
	
	ui.completedWorksTableWidget->clearContents();
	ui.completedWorksTableWidget->setRowCount(0);
	showNotReviewedWorks(ui.notReviewedWorksCheckBox->checkState());
	showReviewedWorks(ui.reviewedWorksCheckBox->checkState());

		//TODO
		//Раскидать код, который ниже, по функциям выше + исправить запрос в зависимости от типа выводимых фактов
		//
		/*ui.completedWorksTableWidget->clearContents();
		ui.completedWorksTableWidget->setRowCount(query.size());
		int i=0;
		while (query.next()) {
			ui.completedWorksTableWidget->setItem(i, 0, new QTableWidgetItem(query.value(0).toString()));
			ui.completedWorksTableWidget->setCellWidget(i, 1, getWorkControlButton(query.value(0).toInt(), i));
			ui.completedWorksTableWidget->setItem(i, 2, new QTableWidgetItem(query.value(7).toString()));
			ui.completedWorksTableWidget->setItem(i, 3, new QTableWidgetItem(query.value(8).toString()));
			ui.completedWorksTableWidget->setItem(i, 4, new QTableWidgetItem(query.value("name").toString()));
			ui.completedWorksTableWidget->setItem(i, 5, new QTableWidgetItem(query.value("numProduct").toString()));
			ui.completedWorksTableWidget->setItem(i, 6, new QTableWidgetItem(query.value("status").toString()));
			i++;
		}*/
}


void ClientWindow::showStartPageStatistic() {
	if (ui.mainWindowStackedWidget->currentIndex() != 2) {
		ui.mainWindowStackedWidget->setCurrentIndex(2);
	}
	ui.createSalaryStatisticButton->setEnabled(true);
	ui.createWorkerStatisticButton->setEnabled(true);
	ui.workerStatisticChoosenStackedWidget->setCurrentIndex(0);
	ui.salaryStatisticChoosenStackedWidget->setCurrentIndex(0);
	setVisibleStatisticButtons(false);
}

void ClientWindow::showStatistic() {

}

void ClientWindow::showSettingsSalaryStatistic() {
	ui.createSalaryStatisticButton->setEnabled(false);
	ui.salaryStatisticChoosenStackedWidget->setCurrentIndex(1);
	setVisibleStatisticButtons(true);
}

void ClientWindow::showSettingsWorkerStatistic() {
	ui.createWorkerStatisticButton->setEnabled(false);
	ui.workerStatisticChoosenStackedWidget->setCurrentIndex(1);
	setVisibleStatisticButtons(true);
}

void ClientWindow::getRegisterWorkerDialog() {
	registerWorkerDialog = new RegisterWorkerDialog(this);
	connect(registerWorkerDialog, SIGNAL(workerRegistered(bool, QString, int, QString, QString, QString)), this, SLOT(setNewWorker(bool, QString, int, QString, QString, QString)));
	registerWorkerDialog->setCurrentTime();
	registerWorkerDialog->show();
}

void ClientWindow::setNewWorker(bool isSuccessRegistration, QString errorMsg, int id, QString fullName, QString dateHiring, QString login) {
	if (isSuccessRegistration) {
		registerWorkerDialog->reject();
		int row = ui.workersTableWidget->rowCount();
		ui.workersTableWidget->insertRow(row);
		ui.workersTableWidget->setItem(row, 0, new QTableWidgetItem(QString().setNum(id)));
		ui.workersTableWidget->setItem(row, 1, new QTableWidgetItem(fullName));
		ui.workersTableWidget->setItem(row, 2, new QTableWidgetItem(login));
		ui.workersTableWidget->setItem(row, 3, new QTableWidgetItem(ucode("В штате")));
		ui.workersTableWidget->setItem(row, 4, new QTableWidgetItem(dateHiring));
		QMessageBox::information(
			this,
			ucode("Регистрация нового сотрудника"),
			ucode("Сотрудник ") + fullName +  ucode(" успешно зарегистрирован в системе!")
			);
	}
	else {
		QMessageBox::warning(
			this,
			ucode("Ошибка регистрации сотрудника"),
			errorMsg
			);
	}
}

void ClientWindow::getCreateProductionTypeDialog() {
	createProductionTypeDialog = new CreateProductionTypeDialog(this);
	connect(createProductionTypeDialog, SIGNAL(productionTypeCreated(int, QString, int)), this, SLOT(setNewProductionType(int, QString, int)));
	createProductionTypeDialog->show();
}

void ClientWindow::setNewProductionType(int id, QString name, int cost) {
	createProductionTypeDialog->reject();
	int row = ui.productionTypesTableWidget->rowCount();

	QWidget* controlButtons = getTypesControlWidget(id);
	ui.productionTypesTableWidget->insertRow(row);
	ui.productionTypesTableWidget->setItem(row, 0, new QTableWidgetItem(QString().setNum(id)));
	ui.productionTypesTableWidget->setItem(row, 1, new QTableWidgetItem(name));
	ui.productionTypesTableWidget->item(row, 1)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	ui.productionTypesTableWidget->setItem(row, 2, new QTableWidgetItem(QString().setNum(cost)));
	ui.productionTypesTableWidget->item(row, 2)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	ui.productionTypesTableWidget->setCellWidget(row, 3, controlButtons);
	ui.productionTypesTableWidget->setRowHeight(row, 50);
}

void ClientWindow::getCreateProductionDialog() {
	createProductionDialog = new CreateProductionDialog(currentUserId, this);
	connect(createProductionDialog, SIGNAL(worksCreated(QString, QList<QPair<int, QPair<QString, int> > >)), this, SLOT(setNewCompletedWorks(QString, QList<QPair<int, QPair<QString, int> > >)));
	createProductionDialog->show();
}

void ClientWindow::setNewCompletedWorks(QString data, QList<QPair<int, QPair<QString, int> > > workList) {
	createProductionDialog->reject();

	int row = ui.productionTableWidget->rowCount();
	int workListCount = workList.size();
	for(int i = 0; i < workListCount; ++i) {
		ui.productionTableWidget->insertRow(row);
		ui.productionTableWidget->setItem(row, 0, new QTableWidgetItem(QString().setNum(workList.at(i).first)));
		ui.productionTableWidget->setItem(row, 1, new QTableWidgetItem(data));
		ui.productionTableWidget->setItem(row, 2, new QTableWidgetItem(workList.at(i).second.first));
		ui.productionTableWidget->setItem(row, 3, new QTableWidgetItem(QString().setNum(workList.at(i).second.second)));
		ui.productionTableWidget->setItem(row, 4, new QTableWidgetItem(ucode("Новая")));
		row++;
	}
}

void ClientWindow::showWorkerData() {
	int row;
	ui.workersTableWidget->selectedItems().isEmpty() ? row = -1 : row = ui.workersTableWidget->selectedItems().at(0)->row();
	if (ui.workerFioLineEdit->isEnabled()) {
		int answer = QMessageBox::question(
			this,
			ucode("Сохранение данных"),
			ucode("Вы действительно хотите завершить редактирование данных сотрудника?\nВсе несохраненные данные будут потеряны!"),
			ucode("Да"),
			ucode("Нет")
			);
		if (answer == 1) {
			int prevRow;
			QList<QTableWidgetItem*> list = ui.workersTableWidget->findItems(QString().setNum(ui.hiddenIdWorkerSpinBox->value()), Qt::MatchFixedString);
			foreach (QTableWidgetItem* item, list) {
				if (item->column() == 0) {
					prevRow = item->row();
					break;
				}
			}
			ui.workersTableWidget->blockSignals(true);
			ui.workersTableWidget->selectRow(prevRow);
			ui.workersTableWidget->blockSignals(false);
			return;
		} 
	}
	if (row != -1) {
		ui.workerInfoStackedWidget->setCurrentIndex(1);
		ui.hiddenIdWorkerSpinBox->setValue(ui.workersTableWidget->item(row, 0)->text().toInt());
		ui.workerFioLineEdit->setText(ui.workersTableWidget->item(row, 1)->text());
		ui.workerLoginLineEdit->setText(ui.workersTableWidget->item(row, 2)->text());
		ui.workerNewPasswordLineEdit->setText("");
		ui.workerStartDateEdit->setDate(QDate::fromString(ui.workersTableWidget->item(row, 4)->text(), "dd.MM.yyyy"));
	}
	else {
		showWorkersPage();
	}
	setDefaultStateWorkerInfoPage(true);
}

void ClientWindow::showEditWorkerInfo() {
	setDefaultStateWorkerInfoPage(false);
}

void ClientWindow::saveEditWorkerInfo() {
	QString fio = ui.workerFioLineEdit->text();
	QString date = ui.workerStartDateEdit->date().toString("dd.MM.yyyy");
	QString login = ui.workerLoginLineEdit->text();
	QString password = ui.workerNewPasswordLineEdit->text();
	if (password.length() > 0 && password.length() < 6) {
		QMessageBox::warning(
			this,
			ucode("Ошибка редактирования данных сотрудника"),
			ucode("Новый пароль должен быть длиной не менее 6 символов!")
			);
		return;
	}
	QString passwordHash = QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Md5));
	int row = ui.workersTableWidget->selectedItems().at(0)->row();
	int id = ui.workersTableWidget->item(row, 0)->text().toInt();

	//TODO
	//Запрос к БД update факта работы
	QSqlQuery query = DataBaseProvider::getQuery();
	QString queryString = QString("UPDATE worker, user SET user.login = ?, user.fio = ?, worker.hiringday = ?");
	if (password.length() != 0) {
		queryString += QString(", user.password = ?");
	}
	queryString += QString(" WHERE worker.id = ? AND user.id = worker.idUser");
	query.prepare(queryString);
	query.addBindValue(login);
	query.addBindValue(fio);
	query.addBindValue(date);
	if (password.length() != 0) {
		query.addBindValue(passwordHash);
	}
	query.addBindValue(id);
	bool success = DataBaseProvider::execQuery(query);

	if (success) {
		int updatedRow = ui.workersTableWidget->selectedItems().at(0)->row();
		ui.workersTableWidget->item(updatedRow, 1)->setText(fio);
		ui.workersTableWidget->item(updatedRow, 2)->setText(login);
		ui.workersTableWidget->item(updatedRow, 4)->setText(date);
		setDefaultStateWorkerInfoPage(true);
	}
}

void ClientWindow::setDefaultStateWorkerInfoPage(bool isOk) {
	ui.workerFioLineEdit->setEnabled(!isOk);
	ui.workerStartDateEdit->setEnabled(!isOk);
	ui.workerLoginLineEdit->setEnabled(!isOk);
	ui.workerPasswordLabel->setVisible(!isOk);
	ui.workerNewPasswordLineEdit->setVisible(!isOk);
	ui.saveWorkerInfoButton->setVisible(!isOk);
	/*ui.showWorkButton->setVisible(isOk);
	ui.setAdjustmentsButton->setVisible(isOk);*/
	ui.editWorkerInfoButton->setVisible(isOk);
}

int ClientWindow::selectOneNumFromDB(const QString& stringQuery) {
	QSqlQuery query = DataBaseProvider::getQuery();
	query.prepare(stringQuery);
	bool success = DataBaseProvider::execQuery(query);
	if(!success || query.next()) {
		//TODO exception
	}
	return query.value(0).toInt();
}

QString ClientWindow::selectOneStringFromDB(const QString& stringQuery) {
	QSqlQuery query = DataBaseProvider::getQuery();
	query.prepare(stringQuery);
	bool success = DataBaseProvider::execQuery(query);
	if(!success || query.next()) {
		//TODO exception
	}
	return QString(query.value(0).toString());
}

void ClientWindow::updateDb(const QString& stringQuery) {
	QSqlQuery query = DataBaseProvider::getQuery();
	query.prepare(stringQuery);
	bool success = DataBaseProvider::execQuery(query);
	if(!success) {
		//TODO exception
	}
}

int ClientWindow::getMaxIdWorker() {
	return selectOneNumFromDB("SELECT MAX(id) FROM worker;");
}

int ClientWindow::getMaxIdBillingPeriod() {
	return selectOneNumFromDB("SELECT MAX(id) FROM billng_period;");
}

int ClientWindow::getIdSalary(const QString& stringCurPeriod, const QString& stringIdWorker) {
	return selectOneNumFromDB("SELECT id FROM salary WHERE idBillingPeriod = " + stringCurPeriod + " AND idWorker = " + stringIdWorker + ";");
}

int ClientWindow::calcSalaryForWorker(int idWorker, const QString& lastPeriod) {
	QString stringIdWorker(QString().setNum(idWorker));

	QString stringQueryCalcSalary("SELECT SUM(numProduct * costType) AS mul FROM fact_of_work, (SELECT type_product.cost AS costType, fact_of_work.id AS idFact FROM type_product, fact_of_work WHERE type_product.id = fact_of_work.idType AND idSalary = ");
	int curId = getIdSalary(lastPeriod, stringIdWorker);

	stringQueryCalcSalary += QString().setNum(curId);

	stringQueryCalcSalary += " AND fact_of_work.status = 1) AS T2 WHERE idSalary = ";

	stringQueryCalcSalary += QString().setNum(curId);

	stringQueryCalcSalary += " AND fact_of_work.status = 1 AND T2.idFact = id;";

	return selectOneNumFromDB(stringQueryCalcSalary);
}

void ClientWindow::saveSalaryForWorker(int idWorker, const QString& lastPeriod, int sizeSalary) {
	QString stringIdWorker(QString().setNum(idWorker));

	int idSalary = getIdSalary(lastPeriod, stringIdWorker);

	updateDb("UPDATE salary SET size = " + QString().setNum(sizeSalary) + " WHERE id = " + QString().setNum(idSalary) + ";");
}

void ClientWindow::сompleteBillingPeriod() {
	int colWorker = getMaxIdWorker();
	QString lastPeriod = QString().setNum(getMaxIdBillingPeriod());

	for(int i = 1; i <= colWorker; ++i) {
		saveSalaryForWorker(i, lastPeriod, calcSalaryForWorker(i, lastPeriod));
	}

	// Закрытие расчетного периода
	int y, m, d;
	QDate::currentDate().getDate(&y, &m, &d);
	QString date(QString().setNum(d) + "." + QString().setNum(m) + "." + QString().setNum(y));
	updateDb("UPDATE billng_period SET endDate = '" + date + "' WHERE id = " + lastPeriod + ";");

	// Открытие нового расчетного периода
	updateDb("INSERT INTO billng_period(startDate) VALUE ('" + date + "');");

	QString newPeriod(QString().setNum(lastPeriod.toInt() + 1));

	//Создание новых салари
	for(int i = 1; i <= colWorker; ++i) {
		QString stringIdWorker(QString().setNum(i));
		updateDb("INSERT INTO salary(idWorker, idBillingPeriod) VALUE (" + stringIdWorker + ", " + newPeriod + ");");
	}
}

void ClientWindow::closeBillingPeriod() {
	int answer = QMessageBox::question(
		this,
		ucode("Сохранение данных"),
		ucode("При закрытии текущего расчетного периода система автоматически расчитает заработную плату для всех сотрудников и откроет новый расчетный период с текущего дня!\n"
		"Вы действительно хотите закрыть текущий расчетный период?"),
		ucode("Да"),
		ucode("Нет")
		);

	if (answer == 0) {
		ui.workersSalaryTableWidget->clearContents();
		ui.workersSalaryTableWidget->setRowCount(0);

		ui.mainWindowStackedWidget->setCurrentIndex(3);
		сompleteBillingPeriod();

		int colWorker = getMaxIdWorker();
		for(int i = 1; i <= colWorker; ++i) {
			QString curSalary(QString().setNum(getIdSalary(QString().setNum(getMaxIdBillingPeriod() - 1), QString().setNum(i))));

			QString fio = selectOneStringFromDB("SELECT fio as idWorker FROM user INNER JOIN worker ON user.id = worker.idUser WHERE worker.id = " + QString().setNum(i) + ";");
			QString sizeSalary = QString().setNum(selectOneNumFromDB("SELECT size FROM salary WHERE id = " + curSalary + ";"));

			ui.workersSalaryTableWidget->insertRow(i - 1);
			ui.workersSalaryTableWidget->setItem(i - 1, 0, new QTableWidgetItem(fio));
			ui.workersSalaryTableWidget->setItem(i - 1, 1, new QTableWidgetItem(sizeSalary));
		}

		setCurrentBillingPeriod();
	}
}

void ClientWindow::showReviewedWorks(int checkState) {
	if (checkState == Qt::Unchecked) {
		QList<QTableWidgetItem*> removingItems = ui.completedWorksTableWidget->findItems(ucode("Принято"), Qt::MatchFixedString);
		removingItems.append(ui.completedWorksTableWidget->findItems(ucode("Отклонено"), Qt::MatchFixedString));
		foreach(QTableWidgetItem* item, removingItems) {
			if (item->column() == 6) {
				ui.completedWorksTableWidget->removeRow(item->row());
			}
		}
	}
	else {
		//TODO
		//Запрос к БД получение списка всех записей со статусом ПРИНЯТО или ОТКЛОНЕНО
		//Отобразить полученный список в ui.completedWorksTableWidget
		QSqlQuery query = DataBaseProvider::getQuery();
		query.prepare("SELECT fact_of_work.id, billng_period.startDate, billng_period.endDate, user.fio, type_product.name, fact_of_work.numProduct, fact_of_work.status FROM fact_of_work INNER JOIN type_product ON fact_of_work.idType = type_product.id AND (fact_of_work.status = 1 OR fact_of_work.status = 2) INNER JOIN salary ON fact_of_work.idSalary = salary.id INNER JOIN billng_period ON salary.idBillingPeriod = billng_period.id INNER JOIN worker ON salary.idWorker = worker.id INNER JOIN user ON worker.idUser = user.id WHERE billng_period.endDate IS NULL OR billng_period.endDate = ''");
		bool success = DataBaseProvider::execQuery(query);

		if (success) {
			int row = ui.completedWorksTableWidget->rowCount();
			while (query.next()) {
				ui.completedWorksTableWidget->insertRow(row);
				ui.completedWorksTableWidget->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
				ui.completedWorksTableWidget->setCellWidget(row, 1, getWorkControlButton(query.value(0).toInt(), row));
				ui.completedWorksTableWidget->setItem(row, 2, new QTableWidgetItem(query.value(1).toString() + QString(" - ") + query.value(2).toString()));
				ui.completedWorksTableWidget->setItem(row, 3, new QTableWidgetItem(query.value(3).toString()));
				ui.completedWorksTableWidget->setItem(row, 4, new QTableWidgetItem(query.value(4).toString()));
				ui.completedWorksTableWidget->setItem(row, 5, new QTableWidgetItem(query.value(5).toString()));
				QString status;
				if (query.value("status").toString().compare("1") == 0) {
					status = ucode("Принято");
				}
				else {
					status = ucode("Отклонено");
				}
				ui.completedWorksTableWidget->setItem(row, 6, new QTableWidgetItem(status));
				row++;
			}
		}
	}
}

void ClientWindow::showNotReviewedWorks(int checkState) {
	if (checkState == Qt::Unchecked) {
		QList<QTableWidgetItem*> removingItems = ui.completedWorksTableWidget->findItems(ucode("Новая"), Qt::MatchFixedString);
		removingItems.append(ui.completedWorksTableWidget->findItems(ucode("Отредактировано"), Qt::MatchFixedString));
		foreach(QTableWidgetItem* item, removingItems) {
			if (item->column() == 6) {
				ui.completedWorksTableWidget->removeRow(item->row());
			}
		}
	}
	else {
		QSqlQuery query = DataBaseProvider::getQuery();
		query.prepare("SELECT fact_of_work.id, billng_period.startDate, billng_period.endDate, user.fio, type_product.name, fact_of_work.numProduct, fact_of_work.status FROM fact_of_work INNER JOIN type_product ON fact_of_work.idType = type_product.id AND (fact_of_work.status = 0 OR fact_of_work.status = 3) INNER JOIN salary ON fact_of_work.idSalary = salary.id INNER JOIN billng_period ON salary.idBillingPeriod = billng_period.id INNER JOIN worker ON salary.idWorker = worker.id INNER JOIN user ON worker.idUser = user.id WHERE billng_period.endDate IS NULL OR billng_period.endDate = ''");
		bool success = DataBaseProvider::execQuery(query);

		if (success) {
			int row = ui.completedWorksTableWidget->rowCount();
			int rowSize = query.size();
			ui.completedWorksTableWidget->setRowCount(row + rowSize);
			while (query.next()) {
				ui.completedWorksTableWidget->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
				ui.completedWorksTableWidget->setCellWidget(row, 1, getWorkControlButton(query.value(0).toInt(), row));
				ui.completedWorksTableWidget->setItem(row, 2, new QTableWidgetItem(query.value(1).toString() + QString(" - ") + query.value(2).toString()));
				ui.completedWorksTableWidget->setItem(row, 3, new QTableWidgetItem(query.value(3).toString()));
				ui.completedWorksTableWidget->setItem(row, 4, new QTableWidgetItem(query.value(4).toString()));
				ui.completedWorksTableWidget->setItem(row, 5, new QTableWidgetItem(query.value(5).toString()));
				QString status;
				if (query.value("status").toString().compare("0") == 0) {
					status = ucode("Новая");
				}
				else {
					status = ucode("Отредактировано");
				}
				ui.completedWorksTableWidget->setItem(row, 6, new QTableWidgetItem(status));
				row++;
			}
		}
	}
}

bool setWorkStatus(int workId, int status)
{
	QSqlQuery query = DataBaseProvider::getQuery();
	query.prepare("UPDATE `fact_of_work` SET `status`=? WHERE id = ?");
	query.addBindValue(status);
	query.addBindValue(workId);
	return DataBaseProvider::execQuery(query);
}

void ClientWindow::acceptWork() {
	QAction* action = qobject_cast<QAction*>(sender());
	int id = action->property("id").toInt();
	int row = action->parentWidget()->parentWidget()->property("row").toInt();

	//TODO
	//Запрос к БД на принятие записи с айдишником id

	QTableWidgetItem* currentWorkStatus = ui.completedWorksTableWidget->item(row, 6);
	QString status = currentWorkStatus->text();
	if (status.compare(ucode("Принято")) == 0) {
		QMessageBox::about(
			this,
			ucode("Управление фактом выполненной работы"),
			ucode("Данный факт выполненной работы уже принят!")
			);
		return;
	}

	if (status.compare(ucode("Отклонено")) == 0) {
		int answer = QMessageBox::question(
			this,
			ucode("Управление фактом выполненной работы"),
			ucode("Данный факт выполненной работы является отклоненным!\nВы действительно хотите принять данный факт выполненной работы"),
			ucode("Да"),
			ucode("Нет")
			);
		if (answer == 0) {
			setWorkStatus(id, 1);
			currentWorkStatus->setText(ucode("Принято"));
		}
	}
	else {
		setWorkStatus(id, 1);
		currentWorkStatus->setText(ucode("Принято"));
	}

	
}

void ClientWindow::rejectWork() {
	QAction* action = qobject_cast<QAction*>(sender());
	int id = action->property("id").toInt();
	int row = action->parentWidget()->parentWidget()->property("row").toInt();

	//TODO
	//Запрос к БД на отклонение записи с айдишником id

	QTableWidgetItem* currentWorkStatus = ui.completedWorksTableWidget->item(row, 6);
	QString status = currentWorkStatus->text();
	if (status.compare(ucode("Отклонено")) == 0) {
		QMessageBox::about(
			this,
			ucode("Управление фактом выполненной работы"),
			ucode("Данный факт выполненной работы уже отклонен!")
			);
		return;
	}

	if (status.compare(ucode("Принято")) == 0) {
		int answer = QMessageBox::question(
			this,
			ucode("Управление фактом выполненной работы"),
			ucode("Данный факт выполненной работы является принятым!\nВы действительно хотите отклонить данный факт выполненной работы"),
			ucode("Да"),
			ucode("Нет")
			);
		if (answer == 0) {
			setWorkStatus(id, 2);
			currentWorkStatus->setText(ucode("Отклонено"));
		}
	}
	else {
		setWorkStatus(id, 2);
		currentWorkStatus->setText(ucode("Отклонено"));
	}
}

void ClientWindow::editProductionType() {
	QPushButton* editButton = qobject_cast<QPushButton*>(sender());
	int id = editButton->parent()->property("id").toInt();
	//Проверить если id находится в колонке 0!!!
	int row;
	QList<QTableWidgetItem*> idList = ui.productionTypesTableWidget->findItems(QString().setNum(id, 10), Qt::MatchFixedString);
	foreach(QTableWidgetItem* item, idList) {
		if (item->column() == 0) {
			row = item->row();
			break;
		}
	}

	ui.productionTypesTableWidget->item(row, 1)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
	ui.productionTypesTableWidget->item(row, 2)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
	QWidget* toolWidget = ui.productionTypesTableWidget->cellWidget(row, 3);
	toolWidget->layout()->itemAt(0)->widget()->setVisible(false);
	//toolWidget->layout()->itemAt(1)->widget()->setVisible(false);
	toolWidget->layout()->itemAt(1)->widget()->setVisible(true);
}

void ClientWindow::deleteProductionType() {
	QPushButton* deleteButton = qobject_cast<QPushButton*>(sender());
	int id = deleteButton->parent()->property("id").toInt();
	//Проверить если id находится в колонке 0!!!
	int row;
	QList<QTableWidgetItem*> idList = ui.productionTypesTableWidget->findItems(QString().setNum(id, 10), Qt::MatchFixedString);
	foreach(QTableWidgetItem* item, idList) {
		if (item->column() == 0) {
			row = item->row();
			break;
		}
	}
	
	//TODO
	//Запрос к БД на delete записи с айдишником id

	ui.productionTypesTableWidget->removeRow(row);
}

void ClientWindow::saveProductionType() {
	QPushButton* saveButton = qobject_cast<QPushButton*>(sender());
	int id = saveButton->parent()->property("id").toInt();
	//Проверить если id находится в колонке 0!!!
	int row;
	QList<QTableWidgetItem*> idList = ui.productionTypesTableWidget->findItems(QString().setNum(id, 10), Qt::MatchFixedString);
	foreach(QTableWidgetItem* item, idList) {
		if (item->column() == 0) {
			row = item->row();
			break;
		}
	}

	//Запрос к БД на update записи с айдишником id
	QString name = ui.productionTypesTableWidget->item(row, 1)->text();
	int cost = ui.productionTypesTableWidget->item(row, 2)->text().toInt();
	QSqlQuery query = DataBaseProvider::getQuery();
	query.prepare("UPDATE type_product SET name = ?, cost = ? WHERE id = ?");
	query.addBindValue(name);
	query.addBindValue(cost);
	query.addBindValue(id);
	DataBaseProvider::execQuery(query);

	ui.productionTypesTableWidget->item(row, 1)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	ui.productionTypesTableWidget->item(row, 2)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	QWidget* toolWidget = ui.productionTypesTableWidget->cellWidget(row, 3);
	toolWidget->layout()->itemAt(0)->widget()->setVisible(true);
	//toolWidget->layout()->itemAt(1)->widget()->setVisible(true);
	toolWidget->layout()->itemAt(1)->widget()->setVisible(false);
}

void ClientWindow::showProductionData() {
	int row;
	ui.productionTableWidget->selectedItems().isEmpty() ? row = -1 : row = ui.productionTableWidget->selectedItems().at(0)->row();
	
	QSqlQuery query = DataBaseProvider::getQuery();
	query.prepare("SELECT id, name FROM type_product");
	DataBaseProvider::execQuery(query);
	ui.productionInfoTitleComboBox->clear();
	while (query.next()) {
		ui.productionInfoTitleComboBox->addItem(query.value(1).toString(), query.value(0).toInt());
	}

	if (ui.productionInfoTitleComboBox->isEnabled()) {
		int answer = QMessageBox::question(
			this,
			ucode("Сохранение данных"),
			ucode("Вы действительно хотите завершить редактирование данных выполненной работы?\nВсе несохраненные данные будут потеряны!"),
			ucode("Да"),
			ucode("Нет")
			);
		if (answer == 1) {
			int prevRow;
			QList<QTableWidgetItem*> list = ui.productionTableWidget->findItems(QString().setNum(ui.hiddenIdWorkSpinBox->value()), Qt::MatchFixedString);
			foreach (QTableWidgetItem* item, list) {
				if (item->column() == 0) {
					prevRow = item->row();
					break;
				}
			}
			ui.productionTableWidget->blockSignals(true);
			ui.productionTableWidget->selectRow(prevRow);
			ui.productionTableWidget->blockSignals(false);
			return;
		} 
	}
	if (row != -1) {
		ui.productionInfoWorkerStackedWidget->setCurrentIndex(1);
		ui.hiddenIdWorkSpinBox->setValue(ui.productionTableWidget->item(row, 0)->text().toInt());
		ui.productionInfoTitleComboBox->setCurrentText(ui.productionTableWidget->item(row, 2)->text());
		ui.productionInfoCountSpinBox->setValue(ui.productionTableWidget->item(row, 3)->text().toInt());
		ui.productionInfoStatusLabel->setText(ui.productionTableWidget->item(row, 4)->text());
	}
	else {
		showMyWorkPage();
	}
	setDefaultStateMyWorkInfoData(true);
	QString statusText = ui.productionInfoStatusLabel->text();
	if (statusText.compare(ucode("Принято")) == 0 || 
		statusText.compare(ucode("Отклонено")) == 0 ||
		statusText.compare(ucode("Удалено")) == 0) {
		ui.productonEditButton->setVisible(false);
		ui.productionDeleteButton->setVisible(false);
	}
	else {
		ui.productonEditButton->setVisible(true);
		ui.productionDeleteButton->setVisible(true);
	}
}

void ClientWindow::setDefaultStateMyWorkInfoData(bool isOk) {
	ui.productionInfoTitleComboBox->setEnabled(!isOk);
	ui.productionInfoCountSpinBox->setEnabled(!isOk);
	ui.productonEditButton->setVisible(isOk);
	ui.productionSaveButton->setVisible(!isOk);
	ui.productionDeleteButton->setVisible(isOk);
}

void ClientWindow::showEditWorkInfo() {
	setDefaultStateMyWorkInfoData(false);
}

void ClientWindow::saveEditWorkInfo() {
	int type = ui.productionInfoTitleComboBox->currentData().toInt();
	int count = ui.productionInfoCountSpinBox->value();
	int row = ui.productionTableWidget->selectedItems().at(0)->row();
	int id = ui.hiddenIdWorkSpinBox->value();

	//TODO
	//Запрос к БД update факта работы
	QSqlQuery query = DataBaseProvider::getQuery();
	query.prepare("UPDATE fact_of_work SET idType = ?, numProduct = ?, status = 3 WHERE id = ?");
	query.addBindValue(type);
	query.addBindValue(count);
	query.addBindValue(id);
	DataBaseProvider::execQuery(query);

	ui.productionInfoStatusLabel->setText(ucode("Отредактировано"));
	ui.productionTableWidget->selectedItems().at(0)->setText(ui.productionInfoTitleComboBox->currentText());
	ui.productionTableWidget->selectedItems().at(1)->setText(QString().setNum(count));
	ui.productionTableWidget->selectedItems().at(2)->setText(ucode("Отредактировано"));

	setDefaultStateMyWorkInfoData(true);
}

void ClientWindow::deleteWorkInfo() {
	int row = ui.productionTableWidget->selectedItems().at(0)->row();
	int id = ui.hiddenIdWorkSpinBox->value();

	//TODO
	//Запрос к БД update факта работы
	QSqlQuery query = DataBaseProvider::getQuery();
	query.prepare("UPDATE fact_of_work SET status = 4 WHERE id = ?");
	query.addBindValue(id);
	DataBaseProvider::execQuery(query);

	ui.productionInfoStatusLabel->setText(ucode("Удалено"));
	ui.productionTableWidget->selectedItems().at(2)->setText(ucode("Удалено"));
}