#include "inventory.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
    
using namespace std;
inventory_table* init_inventory_table(unsigned int cap) {
  inventory_table* ret = new inventory_table;
  ret->table = new itable(cap,nullptr);

  ret->capacity = cap;
  ret->size = 0;
  ret->occupied = 0;

  ret->hash_func = midsquare;
  ret->bucket_func = mod_bucket_func;
 
  
  return ret;
}
inventory_table_2* init_inventory_table2(unsigned int cap) {
  inventory_table_2* ret = new inventory_table_2;

  ret->table2 = new itable2(cap,nullptr);
   
  ret->capacity = cap;
  ret->size = 0;
  ret->occupied = 0;

  ret->hash_func = djb2;
  ret->bucket_func = mod_bucket_func;
  
  return ret;
}


//hash function for table 1
unsigned int midsquare(string key) {

//multiplying inventory number by constant number so that 
//key will not be made up of zeros
    const int primeno = 2729;
    long int key_no;
//convert string key to long integer
    std::stringstream ss(key);
    ss >> key_no;    
//multiplying inventory number by constant number so that 
//key will not be made up of zeros 
    long int p_key = primeno * key_no;    
    long long int squared = p_key * p_key;
//change squared number back to string to 
//extract middle 4 digits
    string squares = to_string(squared);

int num_digits = squares.length();
int mid_point = ((num_digits / 2)-1);
string hash_string = squares.substr(mid_point, 4);
unsigned int hash_no;
//convert the hash string to unsigned int
std::stringstream ss2(hash_string); 
ss2 >> hash_no; 
      
  return hash_no;
}
//hash function for table 2
unsigned int djb2(string key) {

  unsigned int hash = 5381;
  for (size_t i = 0; i < key.length(); i++) {
    char c = key[i];
    hash = ((hash << 5) + hash) + c;
    
  }

  return hash;

}

unsigned int mod_bucket_func(unsigned int hashcode, unsigned int cap) {
  unsigned int b = hashcode % cap;

  return b;
}

inventory_node* init_inventory_node(unsigned int hashcode, inventory_node* inode) {
    inventory_node* ret(new inventory_node);

    ret->inv_key = inode->inv_key;
    ret->hashcode = hashcode;
    ret->component_name = inode->component_name;
    ret->comp_description = inode->comp_description;
    ret->comp_price = inode->comp_price;
    ret->comp_reorder_amount= inode->comp_reorder_amount;
    ret->comp_quant_in_stock = inode->comp_quant_in_stock;
    ret->comp_min_stock_level = inode->comp_min_stock_level;
    ret->comp_man_key = inode->comp_man_key;
    
  return ret;
}
//driver function that inserts nodes into table 1 or 2 using hash functions
bool hashdriver(string key, inventory_node* inode, inventory_table*& tbl, inventory_table_2*& tbl2) {
//cuckoo hash
//attempt to place code in first table and if not successful 
//will continue to displace the original table member until all components are placed appropriately
    
    unsigned int hash;
    unsigned int bucket;
    unsigned int new_capacity;
    inventory_node* temp;
    inventory_node* n_node;
    unsigned int i = 0;
    for (i=0; i < tbl->capacity; i++){
        hash = midsquare(inode->inv_key);
        bucket = mod_bucket_func(hash, tbl->capacity);
        if (tbl->table->at(bucket) == nullptr) {
           
            n_node = init_inventory_node(hash, inode);
            tbl->table->at(bucket) = n_node;
            tbl->size = tbl->size + 1;
    
            tbl->occupied = tbl->occupied + 1;
            return true;
        }
        else
        {   
            temp = init_inventory_node(tbl->table->at(bucket)->hashcode, tbl->table->at(bucket));
        
            tbl->table->at(bucket) = init_inventory_node(hash,inode);
           
            
            //try second table with prior table 1 inventory node
           
            hash = djb2(temp->inv_key);
            bucket = mod_bucket_func(hash, tbl2->capacity);
            if (tbl2->table2->at(bucket) == nullptr) {
                
                n_node = init_inventory_node(hash, temp);
                tbl2->table2->at(bucket) = n_node;
                tbl2->size = tbl2->size + 1;

                tbl2->occupied = tbl2->occupied + 1;
                return true;
            }
            else
            {
                inode = init_inventory_node(tbl2->table2->at(bucket)->hashcode, tbl2->table2->at(bucket));
                
                tbl2->table2->at(bucket) =  init_inventory_node(temp->hashcode, temp);
                
            }
            
        }
    
    }

   // new_capacity = (tbl->capacity * 2);
   // resize(tbl, tbl2, new_capacity);
 
    return false;
 
}



bool change_node_1(inventory_table*& tbl, inventory_node* inode, unsigned int hashcode){
    unsigned int bucket = 0;
 
    bucket = mod_bucket_func(hashcode, tbl->capacity);
    

    if (tbl->table->at(bucket)->inv_key == inode->inv_key) {
            
        tbl->table->at(bucket)->component_name = inode->component_name;
        tbl->table->at(bucket)->comp_description = inode->comp_description;
        tbl->table->at(bucket)->comp_price = inode->comp_price; 
        tbl->table->at(bucket)->comp_reorder_amount= inode->comp_reorder_amount;
        tbl->table->at(bucket)->comp_quant_in_stock = inode->comp_quant_in_stock;
        tbl->table->at(bucket)->comp_min_stock_level = inode->comp_min_stock_level;
        tbl->table->at(bucket)->comp_man_key = inode->comp_man_key;
        
        return true;
        }

    return true;
}
bool change_node_2(inventory_table_2*& tbl, inventory_node* inode, unsigned int hashcode){
    unsigned int bucket = 0;
 
    bucket = mod_bucket_func(hashcode, tbl->capacity);
    

    if (tbl->table2->at(bucket)->inv_key == inode->inv_key) {
            
        tbl->table2->at(bucket)->component_name = inode->component_name;
        tbl->table2->at(bucket)->comp_description = inode->comp_description;
        tbl->table2->at(bucket)->comp_price = inode->comp_price; 
        tbl->table2->at(bucket)->comp_reorder_amount= inode->comp_reorder_amount;
        tbl->table2->at(bucket)->comp_quant_in_stock = inode->comp_quant_in_stock;
        tbl->table2->at(bucket)->comp_min_stock_level = inode->comp_min_stock_level;
        tbl->table2->at(bucket)->comp_man_key = inode->comp_man_key;

        return true;
        }

   
    return true;
}

bool insert_inventory(inventory_table*& tbl, inventory_table_2*& tbl2, inventory_node* inode) 
{   
    unsigned int new_capacity;
    bool completed = false;
    float load_factor = 0.0;
    float load_factor2 = 0.0;
    load_factor = inventory_table_load(tbl);
    load_factor2 = inventory_table_load2(tbl2);
    
    if ((load_factor >= 1) || (load_factor2 >=1)){
      
        new_capacity = tbl->capacity * 2;
        resize(tbl, tbl2, new_capacity);
    }
    
    //decide which table to send to insert_change_node
   
    completed = hashdriver(inode->inv_key, inode, tbl, tbl2);
   
    return completed;
}



float inventory_table_load(inventory_table* tbl) {
    if (tbl == NULL) {
        return 0;
    }
    float load_factor = 0.0;
    if ((tbl->size == 0) || (tbl->capacity == 0)) {
        return 0;
    }
    float fsize = float(tbl->size);
    float fcap = float(tbl->capacity);
    load_factor = (fsize / fcap);
    return load_factor;
}
float inventory_table_load2(inventory_table_2* tbl) {
    if (tbl == NULL) {
        return 0;
    }
    float load_factor = 0.0;
    if ((tbl->size == 0) || (tbl->capacity == 0)) {
        return 0;
    }
    float fsize = float(tbl->size);
    float fcap = float(tbl->capacity);
    load_factor = (fsize / fcap);
    return load_factor;
}
inventory_node* get_value(inventory_table* tbl, inventory_table_2* tbl2, string key) {
    unsigned int hash = 0;
    unsigned int bucket = 0;
    
//check 1st table
    hash = midsquare(key);
    bucket = mod_bucket_func(hash,tbl->capacity);
    if (tbl->table->at(bucket)->inv_key == key) {
        return tbl->table->at(bucket);
    }
    
    //check 2nd table
    hash = djb2(key);
    bucket = mod_bucket_func(hash,tbl2->capacity);
    if (tbl2->table2->at(bucket)->inv_key == key) {
        return tbl2->table2->at(bucket);
    }

  return NULL;
}

struct exists contains(inventory_table* tbl, inventory_table_2* tbl2, string key) {


    struct exists exist_struct;
    exist_struct.in_table = false;
    exist_struct.table_no = 0;
    exist_struct.hashcode = 0;
    unsigned int hash = 0;
    unsigned int bucket = 0;
    

    //check 1st table
    hash = midsquare(key);
        
    bucket = mod_bucket_func(hash,tbl->capacity);
  
    if (tbl->table->at(bucket)!= nullptr) {
         if (tbl->table->at(bucket)->inv_key == key) {
            exist_struct.in_table = true;
            exist_struct.table_no = 1;
            exist_struct.hashcode = bucket;
            return exist_struct;
            }
        }

    //check 2nd table
    hash = djb2(key);
    bucket = mod_bucket_func(hash,tbl2->capacity);
    if (tbl2->table2->at(bucket)!= nullptr) {
        if (tbl2->table2->at(bucket)->inv_key == key) {
            exist_struct.in_table = true;
            exist_struct.table_no = 2;
            exist_struct.hashcode = bucket;
            return exist_struct;
        }
     }
  return exist_struct;
}

bool remove(inventory_table* tbl, inventory_table_2* tbl2, string key) {
    bool removed = false;
    struct exists ex_struct;
    ex_struct = contains(tbl, tbl2, key);
    unsigned int bucket = ex_struct.hashcode;
    
    if (ex_struct.in_table) {
        if (ex_struct.table_no == 1){
            tbl->table->at(bucket) = NULL;
            tbl->size = tbl->size - 1;
            tbl->occupied = tbl->occupied - 1;
            removed = true;
        }
        if (ex_struct.table_no == 2) {
            tbl->table->at(bucket) = NULL;
            tbl2->size = tbl2->size - 1;
            tbl2->occupied = tbl2->occupied - 1;
            removed = true;
        }
    }
    return removed;
   
}

void resize(inventory_table* tbl, inventory_table_2* tbl2, unsigned int new_capacity) {
  bool inserted = false;
  
  string tkey = "";
  //back up table 1
  vector<inventory_node*> temp(tbl->capacity);
  vector<inventory_node*> temp2(tbl->capacity);
 
  unsigned int i = 0;
  unsigned int j = 0;
  for (i=0; i < tbl->capacity; i++) {
      if (tbl->table->at(i)) {
         
        temp[i]->inv_key = tbl->table->at(i)->inv_key;
        temp[i]->hashcode = tbl->table->at(i)->hashcode;
        temp[i]->component_name = tbl->table->at(i)->component_name;
        temp[i]->comp_description = tbl->table->at(i)->comp_description;
        temp[i]->comp_price = tbl->table->at(i)->comp_price; 
        temp[i]->comp_reorder_amount = tbl->table->at(i)->comp_reorder_amount;
        temp[i]->comp_quant_in_stock = tbl->table->at(i)->comp_quant_in_stock;
        temp[i]->comp_min_stock_level = tbl->table->at(i)->comp_min_stock_level;
        temp[i]->comp_man_key =  tbl->table->at(i)->comp_man_key;
      }
      else {temp[i]->inv_key = "";}
  }
  //back up table 2

  j = 0;
  for (j=0; j < tbl2->capacity; j++) {
      if (tbl2->table2->at(j)) {
         
        temp2[j]->inv_key = tbl2->table2->at(j)->inv_key;
        temp2[j]->hashcode = tbl2->table2->at(j)->hashcode;
        temp2[j]->component_name = tbl2->table2->at(j)->component_name;
        temp2[j]->comp_description = tbl2->table2->at(j)->comp_description;
        temp2[j]->comp_price = tbl2->table2->at(j)->comp_price; 
        temp2[j]->comp_reorder_amount = tbl2->table2->at(j)->comp_reorder_amount;
        temp2[j]->comp_quant_in_stock = tbl2->table2->at(j)->comp_quant_in_stock;
        temp2[j]->comp_min_stock_level = tbl2->table2->at(j)->comp_min_stock_level;
        temp2[j]->comp_man_key =  tbl2->table2->at(j)->comp_man_key;
      }
      else {temp2[j]->inv_key = "";}
  }
  delete tbl->table;
  delete tbl2->table2;


  
  tbl->table = new itable(new_capacity,nullptr);
  tbl2->table2 = new itable2(new_capacity,nullptr);
  //insert contents of old table 1 into tables 
  for (i = 0; i < tbl->capacity; i++) {
      if (temp[i]->inv_key != "") {
          tkey = temp[i]->inv_key;
          
          inserted = hashdriver(tkey, temp[i], tbl, tbl2);
            if (!inserted) {
                cout << "problem with resize " << endl;
            }
      }
 
  } 
  tbl->capacity = new_capacity;
  //insert contents of old table 2 into tables
  j = 0;
  for (j = 0; j < tbl2->capacity; j++) {
      if (temp2[j]->inv_key != "") {
            tkey = temp2[j]->inv_key;
            
            inserted = hashdriver(tkey, temp2[j], tbl, tbl2);
            if (!inserted) {
                cout << "problem with resize 2nd loop " << endl;
            }
      }
  } 
  tbl2->capacity = new_capacity;
    return;
}


void print_hash_table(inventory_table* tbl) {
  cout << "Hashtable:" << endl;
  cout << "  capacity: " << tbl->capacity << endl;
  cout << "  size:     " << tbl->size << endl;
  cout << "  occupied: " << tbl->occupied << endl;
  cout << "  load:     " << inventory_table_load(tbl) << endl;
 for (unsigned int i = 0; i < 40; i++) {
      cout << "[" << i << "]    ";
      if (!tbl->table->at(i)) {
        cout << "<empty>" << endl;
      } 
      else {
        cout << "\"" << tbl->table->at(i)->inv_key << "\" = \""
             << tbl->table->at(i)->component_name << "\"" << endl;
         cout    << tbl->table->at(i)->hashcode << endl;
      }
    }
    return;
}

void print_hash_table2(inventory_table_2* tbl2) {
  cout << "Hashtable:" << endl;
  cout << "  capacity: " << tbl2->capacity << endl;
  cout << "  size:     " << tbl2->size << endl;
  cout << "  occupied: " << tbl2->occupied << endl;
  cout << "  load:     " << inventory_table_load2(tbl2) << endl;
 for (unsigned int i = 0; i < 40; i++) {
      cout << "[" << i << "]    ";
      if (!tbl2->table2->at(i)) {
        cout << "<empty>" << endl;
      } 
      else {
        cout << "\"" << tbl2->table2->at(i)->inv_key << "\" = \""
             << tbl2->table2->at(i)->component_name << "\"" << endl;
         cout   << tbl2->table2->at(i)->hashcode << endl;
      }
    }
    return;
}
//updates the inventory table
void update_inventory_table_from_file(string infile, inventory_table*& tbl, inventory_table_2*& tbl2){   

    
    struct exists ex_struct;
    bool updated = false;
    bool inserted = false;
    string key = "";

  
    string field;
    vector<string> fields;
    
    ifstream file(infile);
    // Check if the file is open
    if (!file.is_open()) {
        cout << "Failed to open the file." << std::endl;
        return;
    }

    // Read the file line by line
    string line;
    //dump the header
    getline(file,line);
    
    while (getline(file, line)) {

         stringstream ss3(line);
         fields.clear();
        while (getline(ss3, field, ',')) {
            fields.push_back(field);
          
           
            }
            inventory_node* inode(new inventory_node);
            key = fields[0];
        
            inode->inv_key = key;
            inode->hashcode = 0;
            inode->component_name = fields[2];
            inode->comp_description = fields[3];
            float price = stof(fields[4]);
            inode->comp_price = price;
            int reorder = stoi(fields[5]);
            inode->comp_reorder_amount = reorder;
            int stock = stoi(fields[6]);
            inode->comp_quant_in_stock = stock;
            int stockl = stoi(fields[7]);
            inode->comp_min_stock_level = stockl;
            inode->comp_man_key = stoi(fields[8]);
         
         
          
            updated = false;
            inserted = false;
            ex_struct.in_table = false;
            ex_struct.table_no = 0;
            ex_struct.hashcode = 0;
            ex_struct = contains(tbl, tbl2, inode->inv_key);

            if (ex_struct.in_table) {

               if (ex_struct.table_no == 1){
                   updated = change_node_1(tbl, inode, ex_struct.hashcode);
               }
                else {
         
                    if (ex_struct.table_no == 2){
                    updated = change_node_2(tbl2, inode, ex_struct.hashcode); 
                    }
                    else {
                        updated = false;
                    }
                }
        
                if (!updated) {
                    cout << "problem with update " << field[0] << endl;
                }
            }
            else {
                
                 
                inserted = insert_inventory(tbl, tbl2, inode);
                    if (!inserted) {
                        cout << "problem with insert " << field[0] << endl;
                    }
                }
        delete inode;
        }

    // Close the file
    file.close();
 
 //    print_hash_table(tbl);
  //   print_hash_table2(tbl2);
    return;
}
map<string, struct p_node> product_tab(string infile) {
    
    //product table (map)
    map<string, p_node> p_table;

    string key = "";
    //holds input record
    stringstream ss3;
    string field;
    struct pdetail p1detail;

    //which is the map node
    struct p_node p1node;
    struct p_node p2node;
    struct p_node p3node;
 
    //fields of record being read in
    vector<string> fields;
    vector<struct pdetail> trick_stick;
    vector<struct pdetail> hub_drive;
    vector<struct pdetail> belt_drive;
    
   
    ifstream file(infile);
    // Check if the file is open
    if (!file.is_open()) {
        cout << "Product file failed to open" << std::endl;
        return p_table;
    }
    // Read the file line by line
    string line;
    
    //dump the header
    getline(file,line);

   while (getline(file, line)) {

         stringstream ss3(line);
         fields.clear();
        while (getline(ss3, field, ',')) {
            fields.push_back(field);
        }
        key = fields[0];
        p1detail.comp_key = fields[1];
        p1detail.comp_description = fields[2];
      
        p1detail.comp_no = stoi(fields[3]);
  
        if (key == "1") {
            trick_stick.push_back(p1detail);
        }
        if (key == "2"){
            hub_drive.push_back(p1detail);
        }
        if (key == "3"){
            belt_drive.push_back(p1detail);
        }
    }
// Close the file
  file.close();  
    


    p1node.ptab = trick_stick;
    p1node.product_name = "TRICK-STICK";
    p1node.comp_man_email = "phony_email";
    p_table["1"] = p1node;
    
    p2node.ptab = hub_drive;
    p2node.comp_man_email = "phony_email";
    p2node.product_name = "SHOCBOX Hub Drive";
    p_table["2"] = p2node;
    
     p3node.ptab = belt_drive;
    p3node.comp_man_email = "phony_email";
    p3node.product_name = "SHOCBOX Belt Drive";
    p_table["3"] = p3node;
    


  return p_table;
}
map<int, struct mstruct> build_manufacturer_tab(string infile) {
    
 
    stringstream ss3;
    string field;
    vector<string> fields;
    map<int, mstruct> man_table;
    struct mstruct m1struct;
    string mkey = "";
    
    ifstream file(infile);
    // Check if the file is open
    if (!file.is_open()) {
        cout << "Failed to open the manufacturer file." << std::endl;
        return man_table;
    }

    // Read the file line by line
    string line;
    
    
    //dump the header
    getline(file,line);
    
   while (getline(file, line)) {
         stringstream ss3(line);
        fields.clear();
        while (getline(ss3, field, ',')) {
            fields.push_back(field);
            }

        int key = stoi(fields[0]);
        m1struct.email = fields[1];
        m1struct.name = fields[2];
        m1struct.street_no = fields[3];
        m1struct.street_name = fields[4];
        m1struct.street_name_2 = fields[5];
        m1struct.city = fields[6];
        m1struct.state = fields[7];
        m1struct.country = fields[8];
        m1struct.zip = fields[9];
        m1struct.contact_name = fields[10];
        m1struct.shipping_method = fields[11];
        
        int lead = stoi(fields[12]);
     
        m1struct.lead_time = lead;
      
        man_table[key] = m1struct;
        
    }

    // Close the file
    file.close();

  return man_table;
}

map<string, struct mostruct> build_man_order_tab(string infile) {
    

    stringstream ss3;
    string field;
    vector<string> fields;
    struct mostruct m1struct;
    map<string, mostruct> man_order;
    string key = "";
    
    ifstream file(infile);
    // Check if the file is open
    if (!file.is_open()) {
        cout << "Manufacturer Order file failed to open" << std::endl;
        return man_order;
    }
    // Read the file line by line
    string line;
 
    
    //dump the header
    getline(file,line);
    
      while (getline(file, line)) {
         stringstream ss3(line);
         fields.clear();
        while (getline(ss3, field, ',')) {
            fields.push_back(field);
            }
 
        m1struct.email = fields[1];
        m1struct.order_no = fields[2];
        m1struct.order_item = fields[3];
        m1struct.order_quantity = fields[4];
        m1struct.order_price = fields[5];
        m1struct.street_name = fields[6];
        m1struct.street_name_2 = fields[7];
        m1struct.city = fields[8];
        m1struct.state = fields[9];
        m1struct.country = fields[10];
        m1struct.zip = fields[11];
        m1struct.contact_name = fields[12];
        m1struct.order_date = fields[13];
        m1struct.expect_date = fields[14];
        m1struct.actual_date = fields[15];
        key = fields[0];
        man_order[key] = m1struct;
        
    }


    // Close the file
    file.close();

  return man_order;
}
map<string, struct cstruct> build_cust_info_tab(string infile) {

   
    stringstream ss3;
    string field;
    vector<string> fields;
    struct cstruct c1struct;
    string key;
    
    map<string, struct cstruct> custinfo;

    ifstream file(infile);
    // Check if the file is open
    if (!file.is_open()) {
        cout << "cust file failed to open" << std::endl;
        return custinfo;
    }
    // Read the file line by line
    string line;
 
    
    //dump the header
    getline(file,line);
    
      while (getline(file, line)) {
         stringstream ss3(line);
    
        fields.clear();
        while (getline(ss3, field, ',')) {
            fields.push_back(field);
        }
        c1struct.name = fields[1];
        c1struct.street1 = fields[2];
        c1struct.add1 = fields[3];
        c1struct.add2 = fields[4];
        c1struct.city = fields[5];
        c1struct.zip = fields[6];
        c1struct.state = fields[7];
        c1struct.country = fields[8];
        key = fields[0];
    
        custinfo[key] = c1struct;
        
    }


    // Close the file
    file.close();

  return custinfo;
}
map<string, struct costruct> build_cust_ord_tab(string infile) {

    stringstream ss3;
    string field;
    string c1key = "";
    vector<string> fields;
    struct costruct c1struct;
    
    map<string, struct costruct> cust_order;

    ifstream file(infile);
    // Check if the file is open
    if (!file.is_open()) {
        cout << "cust order file failed to open" << std::endl;
        return cust_order;
    }
    // Read the file line by line
    string line;

    
    //dump the header
    getline(file,line);
     
      while (getline(file, line)) {
         stringstream ss3(line);
        fields.clear();
        while (getline(ss3, field, ',')) {
            fields.push_back(field);
            }
        
        c1struct.email = fields[1];
        float sub = stof(fields[2]);
        c1struct.sub_total = sub;
        float ship = stof(fields[3]);
        c1struct.shipping = ship;
        float tax = stof(fields[4]);
        c1struct.tax = tax;
        c1struct.shipping_method = fields[5];
        int quant = stof(fields[6]);
        c1struct.product_quantity = quant;
        c1struct.product_name = fields[7];
        c1struct.product_price = fields[8];
        c1struct.cust_name = fields[9];
        c1struct.cust_add1 = fields[10];
        c1struct.cust_add2 = fields[11];
        c1struct.city = fields[12];
        c1struct.zip = fields[13];
        c1struct.state = fields[14];
        c1struct.country = fields[15];
        c1key = fields[0];
        if (cust_order.find(c1key) != cust_order.end()){ 
            break;
        }
        else {
            cust_order[c1key] = c1struct;
        }
        
    }

    
    // Close the file
    file.close();

  return cust_order;
}

// Back up the inventory to a CSV
void write_out_inventory(inventory_table* tbl, inventory_table_2* tbl2){   
 
    
            
    string filename = "sbsinventory1.csv";
    std::ofstream outfile(filename);

    if (!outfile) {
        cout << "Failed to open file: " << filename << endl;
        return;
    }

    // Write the header
    outfile << "Key,Hashcode,Name,Description,Price,Reorder Amt,#in Stock,Min Amount,Manufacturer\n";
    int i = 0;
    int j = 0;
    // Write each record to the file
     for (i = 0; i < initial_cap; i++){
         if (tbl->table->at(i) != nullptr){
        
        outfile << tbl->table->at(i)->inv_key<< "," << tbl->table->at(i)->hashcode << "," << tbl->table->at(i)->component_name << "," << tbl->table->at(i)->comp_description << "," << tbl->table->at(i)->comp_price << "," << tbl->table->at(i)->comp_reorder_amount << "," << tbl->table->at(i)->comp_quant_in_stock << "," << tbl->table->at(i)->comp_min_stock_level << "," << tbl->table->at(i)->comp_man_key << "\n";
   
    }
     }
     for (j = 0; j < initial_cap; j++){
         if (tbl2->table2->at(j) != nullptr){
     
        outfile << tbl2->table2->at(j)->inv_key<< "," << tbl2->table2->at(j)->hashcode << "," << tbl2->table2->at(j)->component_name << "," << tbl2->table2->at(j)->comp_description << "," << tbl2->table2->at(j)->comp_price << "," << tbl2->table2->at(j)->comp_reorder_amount << "," << tbl2->table2->at(j)->comp_quant_in_stock << "," << tbl2->table2->at(j)->comp_min_stock_level << "," << tbl2->table2->at(j)->comp_man_key<< "\n";
   
    }
     }
    outfile.close();

}
//**********************************************************
//create data file customer info
void write_out_customer(map<string, struct cstruct> cust){   
            
    string filename = "customer_info1.csv";
    std::ofstream outfile(filename);

    if (!outfile) {
        cout << "Failed to open file: " << filename << endl;
        return;
    }

    // Write the header
     outfile << "Customer Email,Name,Stree1,Address1,Address2,City,State,Country\n";

    // Write each record to the file
     for (std::pair<string, struct cstruct> pair:cust) {
        string key = pair.first;
        struct cstruct cs = pair.second;
        outfile << key << "," << cs.name << "," << cs.street1 << "," << cs.add1 << "," << cs.add2 << "," << cs.city << "," << cs.zip << "," << cs.state << "," << cs.country << "\n";
        }

    outfile.close();
    
}
//write out manufacturer
void write_out_manufacturer(map<int, struct mstruct> man_tab){   
            
    string filename = "manufacturer_info1.csv";
    std::ofstream outfile(filename);

    if (!outfile) {
        cout << "Failed to open file: " << filename << endl;
        return;
    }

    // Write the header
     outfile << "Key, Hash ,Name,Street Number,Street Name,Street Name 2,City,State,Country,Zip,Contact Name,Shipping,Lead Time\n";

    // Write each record to the file
     for (std::pair<int, struct mstruct> pair:man_tab) {
        int key = pair.first;
        struct mstruct mans = pair.second;
      
        outfile << key << "," << mans.street_no << "," << mans.name << "," << mans.street_no << "," << mans.street_name << "," << mans.street_name_2 << "," << mans.city << "," << mans.state << "," << mans.country << "," << mans.zip << "," << mans.contact_name << "," << mans.shipping_method << "," << mans.lead_time <<"\n";
        }

    outfile.close();

}

//******* Decrement from inventory
void decrement_inventory_from_sales(inventory_table* tbl, inventory_table_2* tbl2, string key, int num_sold) {
    unsigned int hash = 0;
    unsigned int bucket = 0;
    
//check 1st table
    hash = midsquare(key);
    bucket = mod_bucket_func(hash,tbl->capacity);
    if (tbl->table->at(bucket)->inv_key == key) {

        tbl->table->at(bucket)->comp_quant_in_stock = (tbl->table->at(bucket)->comp_quant_in_stock - num_sold);
        return;
    }
    
    //check 2nd table
    hash = djb2(key);
    bucket = mod_bucket_func(hash,tbl2->capacity);
    if (tbl2->table2->at(bucket)->inv_key == key) {
  
        tbl2->table2->at(bucket)->comp_quant_in_stock = (tbl2->table2->at(bucket)->comp_quant_in_stock - num_sold);
    
        return;
    }

  return;
}

void update_inventory_from_orders(map<string, struct costruct> ord_map,  map<string, struct p_node> prod_tab, inventory_table*& tbl, inventory_table_2*& tbl2) {
    
    int ts_count = 0;
    int sb_belt = 0;
   // int sb_hub = 0;
    struct p_node pdet;
    int tab_length = 0;
    string key = "";
   
    
    for (std::pair<string, struct costruct> pair:ord_map) {
        string key = pair.first;
        struct costruct orders = pair.second;
        string prod = orders.product_name;
  

    if (prod.substr(0,11) == "TRICK-STICK"){
        ts_count = ts_count + orders.product_quantity;
        
        }
    if (prod.substr(0,12) == "SHOCBOX Belt"){
        sb_belt = sb_belt + orders.product_quantity;
       
        } 
    }

    if (ts_count > 0) {
        if (prod_tab.find(TS_ID) != prod_tab.end()){
            pdet = prod_tab[TS_ID];
        
            tab_length = pdet.ptab.size();
            for (int i = 0; i < tab_length; i++) {
                key = pdet.ptab.at(i).comp_key;
                
                int num_sold = (pdet.ptab.at(i).comp_no * ts_count);
                decrement_inventory_from_sales(tbl, tbl2, key, num_sold);  
          
            }
        }
    }
     if (sb_belt > 0) {
        if (prod_tab.find(SB_BELT_ID) != prod_tab.end()){
            pdet = prod_tab[SB_BELT_ID];
        
            tab_length = pdet.ptab.size();
            for (int i = 0; i < tab_length; i++) {
                key = pdet.ptab.at(i).comp_key;
                
                int num_sold = (pdet.ptab.at(i).comp_no * sb_belt);
                decrement_inventory_from_sales(tbl, tbl2, key, num_sold);  
          
            }
        }
    }
}
//******find lead time
struct reorder_detail find_inv_comp(inventory_table* tbl, inventory_table_2* tbl2, map<int, struct mstruct> man_tab, string key) {
 
    unsigned int hash = 0;
    unsigned int bucket = 0;
    
    struct mstruct m1struct;
    struct reorder_detail r_data;

    
//check 1st table
    hash = midsquare(key);
    
    bucket = mod_bucket_func(hash,tbl->capacity);
  


    if (tbl->table->at(bucket)->inv_key == key) {
        r_data.reorder_amount = tbl->table->at(bucket)->comp_reorder_amount;
        r_data.in_stock = tbl->table->at(bucket)->comp_quant_in_stock;
        r_data.comp_name = tbl->table->at(bucket)->component_name;

        int mankey = tbl->table->at(bucket)->comp_man_key;
        
        if (man_tab.find(mankey) != man_tab.end()){
             m1struct = man_tab[mankey];
             
             r_data.lead_time = m1struct.lead_time;
             return r_data;
        }
        else {
            r_data.lead_time = -1;
            return r_data;
        }
    }
    
    //check 2nd table
    hash = djb2(key);
    bucket = mod_bucket_func(hash,tbl2->capacity);


    if (tbl2->table2->at(bucket)->inv_key == key) {
        r_data.reorder_amount= tbl2->table2->at(bucket)->comp_reorder_amount;
        r_data.in_stock = tbl2->table2->at(bucket)->comp_quant_in_stock;
        r_data.comp_name = tbl2->table2->at(bucket)->component_name;
        int mankey = tbl2->table2->at(bucket)->comp_man_key;
  
        if (man_tab.find(mankey) != man_tab.end()){
             m1struct = man_tab[mankey];
             r_data.lead_time = m1struct.lead_time;
             return r_data;
        }
        else {
            r_data.lead_time = -1;
            return r_data;
        }
    }

  return r_data;
}
map<string, struct reorder_detail> build_reorder_tab(string infile, inventory_table* tbl, inventory_table_2* tbl2, map<int, struct mstruct> man_tab, map<string, struct p_node> prod_tab) {
    
     
    stringstream ss3;
    
    string field;
    vector<string> fields;
    struct reorder_detail re_sub;
    struct p_node pdet;
    struct reorder_data ro_data;
    string key = "";
    int tab_length = 0;
    int temp_sales = 0;
    
    re_sub.avg_day_sales = 0;
    re_sub.lead_time = -1;
    re_sub.reorder_amount = 0;
    re_sub.in_stock = 0;
    
    map<string, struct reorder_detail> reorder_tab;

    ifstream file(infile);
    // Check if the file is open
    if (!file.is_open()) {
        cout << "average daily sales file failed to open" << std::endl;
        return reorder_tab;
    }
    // Read the file line by line
    string line;
    
    //dump the header
    getline(file,line);
    
    while (getline(file, line)) {
        stringstream ss3(line);
    
        fields.clear();
        while (getline(ss3, field, ',')) {
            fields.push_back(field);
        }
        key = fields[0];  
         
        if (prod_tab.find(key) != prod_tab.end()){
            pdet = prod_tab[key]; 
            tab_length = pdet.ptab.size();
            for (int i = 0; i < tab_length; i++) {
                string comp_key = pdet.ptab.at(i).comp_key;
      
                if (reorder_tab.find(comp_key) != reorder_tab.end()) {
      
                    re_sub = reorder_tab[comp_key];
                    temp_sales = stoi(fields[2]);
                    re_sub.avg_day_sales = temp_sales + re_sub.avg_day_sales;
                    reorder_tab[comp_key] = re_sub;
                }
                else
                {  

                   
                   re_sub = find_inv_comp(tbl, tbl2, man_tab, comp_key); 
                   re_sub.avg_day_sales = stoi(fields[2]);
     
                   reorder_tab[comp_key] = re_sub; 
                }
            }
        }
      }

    // Close the file
    file.close();



return reorder_tab;
    
}
void create_reorder_report(map<string, struct reorder_detail> reorder_tab){
    string filename = "reorder.info.csv";
    std::ofstream outfile(filename);

    if (!outfile) {
        cout << "Failed to open file: " << filename << endl;
        return;
    }

    // Write the header
     outfile << "Component Key, Component Name, Avg Day Sales,Lead Time,Amount to Reorder,Current Stock,# Days until Reorder\n";

    // Write each record to the file
     for (std::pair<string, struct reorder_detail> pair:reorder_tab) {
        string key = pair.first;
        struct reorder_detail rd = pair.second;
         //calc the amount of items sold while in transit
        int sold_during_transit =  (rd.avg_day_sales * rd.lead_time);
         //add 10% to that to get min quantity
         int minquant = (1.10 * sold_during_transit);
         int days_from_min = ((rd.in_stock - minquant)/rd.avg_day_sales);
         
        outfile << key << "," << rd.comp_name << "," << rd.avg_day_sales << "," << rd.lead_time << "," << rd.reorder_amount << "," << rd.in_stock << "," << days_from_min << "\n";
        }

    outfile.close();
    cout << "The reorder report was created: " << filename << endl;

    return;
}



