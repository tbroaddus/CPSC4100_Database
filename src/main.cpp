// main.cpp
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <sstream>

// From json11 submodule in lib directory
#include "json11.hpp"
// From fast-cpp-csv-parser submodule in lib directory
#include "csv.h"

#define COLUMN_SIZE 5 

using std::cout;
using std::endl;
using std::cin;



void printSchema(std::fstream& f_csv, 
    const std::vector<json11::Json>& json_vec);


const bool insert(std::fstream& f_csv, 
    const std::vector<json11::Json>& json_vec);




// main()
int main() {


  // Setup

  std::ifstream f_schema;
  // Using relative path since the working directory
  // for the executable is not the source directory.
  // I am sure there is a better way to do this.
  f_schema.open("../../src/schema.json");
  std::string schema_str((std::istreambuf_iterator<char>(f_schema)),
      (std::istreambuf_iterator<char>()));
  f_schema.close();

  std::string err;
  const auto json = json11::Json::parse(schema_str, err);
  std::vector<json11::Json> json_vec;
  for (auto &k : json["fields"].array_items()) {
    std::string temp = k.dump();
    const auto temp_json = json11::Json::parse(temp, err);
    json_vec.push_back(temp_json);
  }
 
  std::fstream f_csv;
  // Checking if there is a data.csv file
  {
    std::ifstream check_file;
    check_file.open("../../src/data.csv");
    if (check_file) {
      check_file.close();
      f_csv.open("../../src/data.csv", std::ios::app);
    } else {
      check_file.close();
      std::ofstream created_file("../../src/data.csv");
      f_csv.open("../../src/data.csv", std::ios::app);
    }
  }
 


  cout << "\n\n\n\n";
  cout << "\t========= Welcome to dumbDB =========\n";
  cout << "\n\n" << endl;
 


  // Main user loop
  std::string cmd; 
  while(true) {

    cout << "\tPlease select one of the following commands: \n";
    cout << "\t\t- insert\n";
    cout << "\t\t- print\n";
    cout << "\t\t- quit\n";
    cout << "\tEnter here: ";
    getline(cin, cmd);
    cout << endl;


    if (cmd == "insert") {
      bool result = insert(f_csv, json_vec); 
    }
    else if (cmd == "print") {
      printSchema(f_csv, json_vec); 
    }
    else if (cmd == "delete") {
    
    } 
    else if (cmd == "quit") {
      break; 
    }
    else {
      cout << "\tPlease enter a valid command" << endl; 
      continue;
    }
    
  }
  
  cout << "Goodbye!" << endl;
  return 0;
}

void printSchema(std::fstream& f_csv, 
    const std::vector<json11::Json>& json_vec) {
  cout << "\n\n\n\n" << "\t=====Data Table=====\n\t";
  for (json11::Json k : json_vec) {
    cout << k["name"].string_value(); 
    cout << "\t|\t";
  }
  cout << endl;
  cout << "\t";
  for (json11::Json k : json_vec) {
    cout << k["type"].string_value(); 
    cout << "\t|\t";
  }
  cout << "\n\n";
  io::CSVReader<COLUMN_SIZE> in("../../src/data.csv");
  while(true) {

    std::string temp = in.next_line();
    if (temp == "")
      break;
    std::stringstream str_stream(temp);
    std::string element;
    std::vector<std::string> element_vec;
    char delim = ',';
    while(std::getline(str_stream, element, delim)) {
      if (element == "")
        break;
      cout << "\t";
      cout << element; 
    }
    cout << endl;
  }


  cout << "\n\n\n" << endl; 
}




const bool insert(std::fstream& f_csv, 
    const std::vector<json11::Json>& json_vec) {

  cout << "\tPlease enter a value for the following attributes: \n";
  std::string line;
  for (json11::Json k : json_vec) {
    cout << "\t\t" << k["name"].string_value() << ": ";  
    if (k["type"].string_value() == "float") {
      float val;
      cin >> val;
      cin.clear();
      cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      line.append(std::to_string(val));
      line.append(",");
    } 
    else if (k["type"].string_value() == "integer") {
      int val;
      cin >> val;
      cin.clear();
      cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      line.append(std::to_string(val));
      line.append(",");
    }
    else if (k["type"].string_value() == "string") {
      std::string val;
      cin >> val;
      cin.clear();
      cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      line.append(val);
      line.append(",");
    }
    else if (k["type"].string_value() == "boolean") {
      std::string val;
      cin >> val;
      cin.clear();
      cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      if (val != "true" && val != "false") {
        cout << "I am returning" << endl;
        return false; 
      }
      line.append(val);
      line.append(",");
    }
    cout << endl;
  }
  line.append("\n");
  f_csv << line;
  return true;
}

