#include "registerworkerdialog.h"
#include "clientwindow.h"

RegisterWorkerDialog::RegisterWorkerDialog(QWidget *parent) : QDialog(parent) {
	codec = QTextCodec::codecForLocale();
	ui.setupUi(this);
	setModal(true);
	//Коммит в своей ветке

	connect(ui.registerButton, SIGNAL(clicked()), this, SLOT(registerWorker()));
	connect(ui.cancelButton, SIGNAL(clicked()), this, SLOT(cancelRegister()));
}

RegisterWorkerDialog::~RegisterWorkerDialog() {

}

void RegisterWorkerDialog::setCurrentTime() {
	ui.dateHiringDateEdit->setDate(QDate::currentDate());
}

void RegisterWorkerDialog::cancelRegister() {
	reject();
}

//Комментарий
void RegisterWorkerDialog::registerWorker() {
	QString errorMsg = QString();
	if (ui.fioLineEdit->text().isEmpty()) {
		errorMsg += codec->toUnicode("Пустое ФИО! Введите ФИО сотрудника!\n");
	}

	if (ui.loginLineEdit->text().isEmpty()) {
		errorMsg += codec->toUnicode("Пустой логин! Введите логин!\n");
	}

	if (ui.passwordLineEdit->text().isEmpty()) {
		errorMsg += codec->toUnicode("Пустой пароль! Введите пароль!\n");
	}

	if (!errorMsg.isEmpty()) {
		QMessageBox::warning(
			this, 
			codec->toUnicode("Ошибка создания"),
			errorMsg
			);
		return;
	}

	QString fio = ui.fioLineEdit->text();
	QString date = ui.dateHiringDateEdit->date().toString("dd.MM.yyyy");
	QString login = ui.loginLineEdit->text();
	QString passwordHash = QString(QCryptographicHash::hash(ui.passwordLineEdit->text().toUtf8(), QCryptographicHash::Md5));
	
	//Новый комментарий
	int i = 0;

	//Запрос к БД
	//Еще комментарий
	QSqlQuery querySelect = DataBaseProvider::getQuery();
	querySelect.prepare("SELECT id FROM user WHERE login = ?");
	querySelect.addBindValue(login);
	DataBaseProvider::execQuery(querySelect);

	if (querySelect.size() > 0) {
		QMessageBox::critical(
			this,
			codec->toUnicode("Ошибка регистрации"),
			codec->toUnicode("Сотрудник с данным логином уже существует!")
			);
		return;
	}

	//Еще комментарий
	QSqlQuery query = DataBaseProvider::getQuery();
    query.prepare("INSERT INTO user (login, password, fio) VALUES (?, ?, ?)");
    query.addBindValue(login);
    query.addBindValue(passwordHash);
	query.addBindValue(fio);
	bool success = DataBaseProvider::execQuery(query);

	QVariant userId = query.lastInsertId();
	query.prepare("INSERT INTO worker (hiringday, idUser) VALUES (?, ?)");
	query.addBindValue(date);
    query.addBindValue(userId);
	success = DataBaseProvider::execQuery(query);
	
	//Комментарий
	int i = 0;
	

	QString stringIdWorker(QString().setNum(ClientWindow::getMaxIdWorker()));
	QString newPeriod(QString().setNum(ClientWindow::getMaxIdBillingPeriod()));

	QSqlQuery query1 = DataBaseProvider::getQuery();
	query1.prepare("INSERT INTO salary(idWorker, idBillingPeriod) VALUE (" + stringIdWorker + ", " + newPeriod + ");");
	success = DataBaseProvider::execQuery(query1);
	if(!success) {
		//TODO exception
	}

	QString errorMessage = codec->toUnicode("");
	emit workerRegistered(true, errorMessage, userId.toInt(), fio, date, login);
}
