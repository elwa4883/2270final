#ifndef INVENTORY_H__
#define INVENTORY_H__

#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <map>

using namespace std;
const unsigned int initial_cap = 40;
const string TS_ID = "1";
const string SB_BELT_ID = "2";
const string SB_HUB_ID = "3";

struct inventory_node{
    string inv_key;
    unsigned int hashcode;
    string component_name;
    string comp_description;
    float comp_price;
    int comp_reorder_amount;
    int comp_quant_in_stock;
    int comp_min_stock_level;
    int comp_man_key;
};

struct exists {
    bool in_table;
    int table_no;
    unsigned int hashcode;
};
struct avg_sales {
    int prod_key;
    string prod_name;
    int avg_daily_sales;
};
struct reorder_data {
    string comp_key;
    string comp_name;
    int avg_day_sales;
    int lead_time;
    int reorder_amount;
    int in_stock;
};
struct reorder_detail {
    int avg_day_sales;
    string comp_name;
    int lead_time;
    int reorder_amount;
    int in_stock;
};

struct pdetail {
    string comp_key;
    string comp_description;
    int comp_no;
};
typedef vector<struct pdetail> prod_tab;

 struct product_node{
    int product_key;
    string product_name;
    string comp_man_email;
    prod_tab ptab;
};  
 struct p_node{
    string product_name;
    string comp_man_email;
    prod_tab ptab;
};
struct manufacturer_struct {
  int key;
  string man_email;
  string man_name;
  string man_street_number;
  string man_street_name;
  string man_street_name_2;
  string man_city;
  string man_state;
  string man_country;
  string man_zip;
  string man_contact_name;
  string man_shipping_method;
  int man_lead_time;
    
};
struct mstruct {
        string email;
        string name;
        string street_no;
        string street_name;
        string street_name_2;
        string city;
        string state;
        string country;
        string zip;
        string contact_name;
        string shipping_method;
        int lead_time;
};
struct man_order_struct {

  // key is the key the user provides to associate with a value.
  string man_order_key;
  string man_email;
  string man_order_no;
  string man_order_item;
  string man_order_quantity;
  string man_order_price;
  string man_street_name;
  string man_street_name_2;
  string man_city;
  string man_state;
  string man_country;
  string man_zip;
  string man_contact_name;
  string man_order_date;
  string man_expect_date;
  string man_actual_date;
    
};
struct mostruct {
        string email;
        string order_no;
        string order_item;
        string order_quantity;
        string order_price;
        string street_name;
        string street_name_2;
        string city;
        string state;
        string country;
        string zip;
        string contact_name;
        string order_date;
        string expect_date;
        string actual_date;
};   
struct customer_info_struct {

  // key
  string cust_email;
  string cust_name;
  string cust_street1;
  string cust_add1;
  string cust_add2;
  string cust_city;
  string cust_zip;
  string cust_state;
  string cust_country;
};
struct cstruct {
        string email;
        string name;
        string street1;
        string add1;
        string add2;
        string city;
        string zip;
        string state;
        string country;
    };
struct customer_order_struct {

  // key is the key the user provides to associate with a value.
  string cust_order_number;
  string cust_order_email;
  float cust_subtotal;
  float cust_shipping;
  float cust_tax;
  string cust_shipping_method;
  int product_quantity;
  string product_name;
  string product_price;
  string cust_name;
  string cust_street1;
  string cust_add1;
  string cust_add2;
  string cust_city;
  string cust_zip;
  string cust_state;
  string cust_country;
  
};

struct costruct {
        string email;
        float sub_total;
        float shipping;
        float tax;
        string shipping_method;
        int product_quantity;
        string product_name;
        string product_price;
        string cust_name;
        string cust_add1;
        string cust_add2;
        string city;
        string zip;
        string state;
        string country;
};

typedef vector<inventory_node*> itable;
typedef vector<inventory_node*> itable2;

struct inventory_table {
  // capacity current number of addressable buckets in table. kept up-to-date.
  unsigned int capacity;

  // number of actual (not deleted) entries. kept up-to-date.
  unsigned int size;

  // number of non-null buckets. (so all hash_nodes, deleted or otherwise). kept
  // up-to-date.
  unsigned int occupied;
  itable* table;
  unsigned int (*hash_func)(string);
  unsigned int (*bucket_func)(unsigned int hashcode, unsigned int capacity);
};
struct inventory_table_2 {
  // capacity current number of addressable buckets in table. kept up-to-date.
  unsigned int capacity;

  // number of actual (not deleted) entries. kept up-to-date.
  unsigned int size;

  // number of non-null buckets. (so all hash_nodes, deleted or otherwise). kept
  // up-to-date.
  unsigned int occupied;
  itable2* table2;
  unsigned int (*hash_func)(string);
  unsigned int (*bucket_func)(unsigned int hashcode, unsigned int capacity);
};


//2hash functions
unsigned int djb2(string key);
unsigned int midsquare(string key);
bool hashdriver(string key, inventory_node* inode, inventory_table tbl, inventory_table_2 tbl2);
unsigned int mod_bucket_func(unsigned int hashcode, unsigned int cap);

inventory_table* init_inventory_table(unsigned int cap);
inventory_table_2* init_inventory_table2(unsigned int cap);
inventory_node* init_inventory_node(unsigned int hashcode, inventory_node* inode);
inventory_node* resize_init_inventory_node(unsigned int hashcode, inventory_node* inode);
float inventory_table_load(inventory_table* tbl);
float inventory_table_load2(inventory_table_2* tbl);
bool set_key(inventory_table* tbl, inventory_table_2* tbl2, inventory_node *inode);
bool insert_inventory(inventory_table*& tbl, inventory_table_2*& tbl2, inventory_node* inode);

inventory_node* get_value(inventory_table* tbl, string key);

bool contains(inventory_table* tbl, string key);
bool remove(inventory_table* tbl, string key);
void resize(inventory_table* tbl, inventory_table_2* tbl2, unsigned int new_capacity);

void print_hash_table(inventory_table* tbl);
void write_out_inventory(inventory_table* tbl, inventory_table_2* tbl2);
bool fill_inventory_table(inventory_table*& tbl);
void update_inventory_table_from_file(string infile, inventory_table*& tbl, inventory_table_2*& tbl2);
map<string, struct p_node> product_tab(string infile);
map<int, struct mstruct> build_manufacturer_tab(string infile);
map<string, struct mostruct> build_man_order_tab(string infile);
map<string, struct cstruct> build_cust_info_tab(string infile);
map<string, struct costruct> build_cust_ord_tab(string infile);
bool prompt_user_selection(inventory_table*& tbl, inventory_table_2*& tbl2, map<string, struct reorder_detail> ro_tab, map<string, struct costruct> ord_tab, map<string, struct p_node> prod_tab);
void create_reorder_report(map<string, struct reorder_detail> reorder_tab);
void create_inventory_report();
void create_reorder_report();
void write_out_customer(map<string, struct cstruct> cust);
void write_out_manufacturer(map<int, struct mstruct> man_tab);
void update_inventory_from_orders(map<string, struct costruct> ord_map,  map<string, struct p_node> prod_tab, inventory_table*& tbl, inventory_table_2*& tbl2);
map<string, struct reorder_detail> build_reorder_tab(string infile, inventory_table* tbl, inventory_table_2* tbl2, map<int, struct mstruct> man_tab, map<string, struct p_node> prod_tab);
struct reorder_detail find_inv_comp(inventory_table* tbl, inventory_table_2* tbl2, map<int, struct mstruct> man_tab, string key);
#endif 
