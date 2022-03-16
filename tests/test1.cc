#include <cstdlib>
#include <cstring>

#include <iomanip>
#include <iostream>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <map>

#include <sqlite3.h>

namespace {

void sqlite_throws(int rc) {
  if (rc == SQLITE_OK)
    return;
  throw std::runtime_error(::sqlite3_errstr(rc));
}
}

int main(int argc, char* argv[]) try {
    std::string db_filename;
    std::string sql = "SELECT COUNT(*) FROM student";
    if (argc > 1)
        db_filename.assign(argv[1]);
    else {
        char const *srcdir = ::getenv("srcdir");
        if (!srcdir)
            throw std::runtime_error("No sqlite database specified!");
        db_filename.assign(std::string(srcdir) + "/tests/data/demo.sqlcipher");
    }
    std::string db_password = "1234";
    ::sqlite3 *db = NULL;
    ::sqlite3_stmt *stmt = NULL;
    sqlite_throws(::sqlite3_open(db_filename.c_str(), &db));
#ifdef SQLITE_HAS_CODEC
    // password
    sqlite_throws(::sqlite3_key(db, db_password.c_str(), db_password.length()));
#endif
    sqlite_throws(::sqlite3_prepare(db, sql.c_str(), sql.length(),  &stmt, NULL));
    // has result set
    if (::sqlite3_column_count(stmt) < 1)
        throw std::runtime_error("no result set of select-count statement");
    // move to first row
    if (SQLITE_ROW != ::sqlite3_step(stmt))
        throw std::runtime_error("no records!");
    // first column starts from zero
    sqlite3_int64 count = ::sqlite3_column_int64(stmt, 0);
    std::cout << "sql = " << sql << " = " << count << std::endl;
    if (count != 1)
        throw std::runtime_error("should be one!");
    sqlite_throws(::sqlite3_finalize(stmt));
    sqlite_throws(::sqlite3_close(db));
    db = NULL;

    return EXIT_SUCCESS;
} catch (std::exception const &e) {
    std::cerr << "[c++ exception] " << e.what() << std::endl;
    return EXIT_FAILURE;
} catch (...) {
    std::cerr << "[c++ exception] " << "<UNKNOWN CAUSE>" << std::endl;
    return EXIT_FAILURE;
}
