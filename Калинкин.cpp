#include "DataBaseProvider.h"

DataBaseSettings DataBaseProvider::settings;

bool DataBaseProvider::execQuery(QSqlQuery query) {
	bool result = false;
	try {
		if(!settings.db.isOpen())
			settings.db.open();
		result = query.exec();
		
		if(!result) throw new Exception("data was corrupted");
	}
	catch(e){
		//TODO
	}

	return result;
}

QSqlQuery DataBaseProvider::getQuery() {
	return QSqlQuery(settings.db);
}

QSqlQuery DataBaseProvider::fetch(){
	
	return DataBaseProvider::Query("select a, b from table1");
}