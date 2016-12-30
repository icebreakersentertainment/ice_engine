#ifndef VERSION_H_
#define VERSION_H_

#include "utilities/Macros.hpp"

#include "data_store/ISqliteSerializable.hpp"

// Forward declaration
namespace pyliteserializer
{
	class SqliteDataStore;
}

// @include Version.hpp
namespace game
{

// @table version
// @namespace { game }
class Version : public pyliteserializer::ISqliteSerializable
{
public:
	Version(int id = 0, int version = 0);	
	virtual ~Version();
	
	// @serialize start
	virtual void serialize(SQLite::Database& db, pyliteserializer::SqliteDataStore& ds);
	// @serialize end

	// @deserialize start
	virtual void deserialize(SQLite::Database& db, pyliteserializer::SqliteDataStore& ds);
	// @deserialize end
	
	// @deserialize_where start
	virtual void deserialize(SQLite::Database& db, pyliteserializer::SqliteDataStore& ds, const std::string& where);
	// @deserialize_where end
	
	// @deserialize_from_query start
	virtual void deserialize(SQLite::Statement& query, pyliteserializer::SqliteDataStore& ds);
	// @deserialize_from_query end

	GETSET(int, id_, Id)
	GETSET(int, version_, Version)

private:
	// @column id
	// @id
	int id_;
	
	// @column version
	int version_;
};

}

#endif /* VERSION_H_ */
