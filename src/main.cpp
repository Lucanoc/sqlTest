#include <exception>
#include <iostream>
#include <pqxx/connection.hxx>
#include <pqxx/nontransaction.hxx>
#include <pqxx/pqxx>
#include <pqxx/result.hxx>
#include <pqxx/transaction.hxx>
#include <string>

auto main() -> int {
    try {
        pqxx::connection dbConn("dbname=testing user=postgres password=1212 hostaddr=127.0.0.1 port=5432");

        if (dbConn.is_open()) {
            std::cout << "opened! " << dbConn.dbname() << '\n';
        } else {
            std::cout << "not opened!\n";

            return -1;
        }

        std::string deleteStatemnt(
            "delete from studentinfos where gender = 'female';"
        );
        std::string selectStatement(
            "select * from studentinfos;"
        );

        pqxx::work wk(dbConn);
        pqxx::result res(wk.exec(selectStatement));
        for (auto const & elem : res) {
            std::cout << "id " << elem[0].as<int>() << ' ';
            std::cout << "score " << elem[1].as<int>() << ' ';
            std::cout << "name " << elem[2].as<std::string>() << ' ';
            std::cout << "gender " << elem[3].as<std::string>() << '\n';
        }
        std::cout << "select successfully!\n";

        wk.exec(deleteStatemnt);
        std::cout << "delete successfully!\n";

        res = wk.exec(selectStatement);
        for (auto const & elem : res) {
            std::cout << "id " << elem[0].as<int>() << ' ';
            std::cout << "score " << elem[1].as<int>() << ' ';
            std::cout << "name " << elem[2].as<std::string>() << ' ';
            std::cout << "gender " << elem[3].as<std::string>() << '\n';
        }
        std::cout << "select successfully!\n";

        wk.commit();
        dbConn.disconnect();
    } catch (std::exception & e) {
        std::cerr << e.what() << std::endl;

        return 1;
    }
    
    return 0;
}