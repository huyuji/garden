// c++10.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <string>
#include <pqxx/pqxx>
#include <boost/smart_ptr/shared_ptr.hpp>

pqxx::result f()
{
    pqxx::connection conn("user='postgres' password='admin' host='10.32.177.160' port='5432' dbname='obs'");
    pqxx::work txn(conn);

    boost::shared_ptr<pqxx::prepare::invocation> invoc = 
        boost::shared_ptr<pqxx::prepare::invocation>(new pqxx::prepare::invocation(txn.prepared("query")));
    if(!invoc->exists())
    {
      conn.prepare("query", "select stat_path_name from obsrv.stat_def where id=$1 and stat_path_name=$2")("integer")("varchar", pqxx::prepare::treat_string);
    }
    invoc = boost::shared_ptr<pqxx::prepare::invocation>(new pqxx::prepare::invocation((*invoc)(1)));
    invoc = boost::shared_ptr<pqxx::prepare::invocation>(new pqxx::prepare::invocation((*invoc)("cifs")));
    return invoc->exec();
}

void testf()
{
  pqxx::result result = f();
  std::cout << result[0]["stat_path_name"].as<std::string>();
}

pqxx::connection conn("user='postgres' password='admin' host='10.32.177.160' port='5432' dbname='obs'");

pqxx::result g1(pqxx::connection& conn)
{
    pqxx::work txn(conn);

    //pqxx::prepare::invocation invoc = txn.prepared("query");
    if(!txn.prepared("query").exists())
    {
      std::cout << "prepare\n";
      conn.prepare("query", "select stat_path_name from obsrv.stat_def where id=$1 and stat_path_name=$2")("integer")("varchar", pqxx::prepare::treat_string);
    }
    pqxx::result result = txn.prepared("query")(1)("cifs").exec();
    txn.commit();
    return result;
}

pqxx::result g2(pqxx::connection& conn)
{
  pqxx::result result = g1(conn);
  return result;
}


pqxx::result g3(pqxx::connection& conn)
{
  pqxx::result result = g2(conn);
  return result;
}

void g4()
{
pqxx::result result = g3(conn);
std::cout << result[0]["stat_path_name"].as<std::string>() << std::endl;

}

void testg()
{
  g4();
  g4();
}

void test1()
{
  try
  {
    pqxx::connection conn("user='metrics' dbname='metricsdb'");
    pqxx::work txn(conn);
    txn.exec("select * from stat_sample_view_5sec where time_stamp in (select time_stamp from stat_sample_view_5sec);");
  }
  catch(const std::exception& e)
  {
    std::cout << e.what() << std::endl;
  }
}

void copy()
{
  pqxx::connection conn("user='postgres' password='admin' host='10.32.179.135' port='5432' dbname='postgres'");

  std::string TableName = "object";
  std::vector<std::string> columnNames;

  columnNames.push_back("object_id");

  columnNames.push_back("object_type_id");

  columnNames.push_back("name");

  pqxx::work P(conn);

  pqxx::tablewriter W(P, TableName, columnNames.begin(), columnNames.end());

  std::vector<std::string> dataForColumns;

  dataForColumns.push_back("8");

  dataForColumns.push_back("1");

  dataForColumns.push_back("client6");

  W.push_back(dataForColumns); // push the data into the tablewriter
  W.complete();

  P.commit();
}

int main()
{
  //testf();
  //testg();
  test1();
  return 0;
}

