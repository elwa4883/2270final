
//main.cpp
//CSPB 2270 final project
#include "../code/inventory.h"


#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <string>
#include <cctype>
#include <cstdlib>

using namespace std;

//defines a structure that will hold a user selection and file name
    struct Input_Values
{ 
    string user_selection;
    string user_input_file_name;
};

//


bool process_sel_1(string user_sel_p)
{

    bool valid = true;

    if ((user_sel_p.length() > 1)  || (user_sel_p.length() < 1))
    {
            valid = false;
            cout << "Please, enter selection 1, 2, 3 or Q to quit" << endl;
    }
    else {
        if ((user_sel_p.substr(0,1) == "1") ||
            (user_sel_p.substr(0,1) == "2") ||
            (user_sel_p.substr(0,1) == "3") ||
            (user_sel_p.substr(0,1) == "Q") ||
            (user_sel_p.substr(0,1) == "q"))
            {
                valid = true;
            } 
            else 
            {
                valid = false;
            }
        }

    return valid;
}
string sel_1_menu()
{
    
    string menu_sel;
    
    cout << "Reports Menu" << endl;
    cout << "1) Create Inventory Report"  << endl;
    cout << "2) Create Report of Items to Reorder" << endl;
    cout << "3) Return to Main Menu"<< endl;
    cout << "Enter menu selection (Q to quit): ";
    cin >> menu_sel;
    
    
    return menu_sel;
}
void sel_1(inventory_table* tbl, inventory_table_2* tbl2, map<string, struct reorder_detail> ro_tab, map<string, struct costruct> ord_tab, map<string, struct p_node> prod_tab)
{
//upload current table files and create maps and tables
//update maps and tables
//update table files
    
    bool done = false;

    
// define user selection string
    string user_sel;
   
// loop to validate user selection   
    while (!done)
    {
        user_sel = sel_1_menu();
        done = process_sel_1(user_sel);
    }
    
    if (user_sel.length() ==1) 
    {
        if ((user_sel.substr(0,1) == "Q") || (user_sel.substr(0,1) == "q"))
        {
          cout << "Quitting...: "<< endl;
          return;
        }
    }
    
    if (user_sel.length() == 1)
    {
        if (user_sel.substr(0,1) == "1")
        {
          write_out_inventory(tbl, tbl2);
          cout << "Inventory Report in: sbsinventory1.csv" << endl;
       
            bool menudone = false;
            menudone = prompt_user_selection(tbl, tbl2, ro_tab, ord_tab, prod_tab);
            return;
        }
        else if (user_sel.substr(0,1) == "2")
        {
            create_reorder_report(ro_tab);
            return;
        }
        else if (user_sel.substr(0,1) == "3")
        {
            bool menudone = false;
            menudone = prompt_user_selection(tbl, tbl2, ro_tab, ord_tab, prod_tab);
            return;
        }
        
    }

}


//prompt the user for the input file name
Input_Values get_file_name()
{
    bool done = false;
    bool endfilename = false;
    string infile_name = "";
    long unsigned int i = 0;
    string u_input;
    string ival = "";
    string ival1 = "";
    string ival2 = "";
    string ival3 = "";
    Input_Values user_input_struct;
    user_input_struct.user_selection = "0";
    user_input_struct.user_input_file_name = "";
    while (!done) {
        infile_name = "";
        cout << "Enter new input CSV filename: ";
        cin >> infile_name;
        i = 0;
        while ((!endfilename) && (i < infile_name.length()))
        {
          
            ival = infile_name.substr(i,1);
           
            if ((ival == ".") && ((i+3) < infile_name.length())) {
              ival1 = infile_name.substr(i+1,1);
              ival2 = infile_name.substr(i+2,1);
              ival3 = infile_name.substr(i+3,1);
              endfilename = true;
            }
            i ++;
        }
       
    
        if (((ival1 == "c") || (ival1 == "C")) &&
           ((ival2 == "s") || (ival2 == "S")) &&
           ((ival3 == "v") || (ival3 == "V")))
                {
                 done = true;
                 user_input_struct.user_input_file_name = infile_name;
                }
        
            else {
                    cout << "Your filename is not correct, would you like to quit?, enter Y or N" << endl;
                  cin >> u_input;
                  if ((u_input.length() == 1) && ((u_input == "Y") ||
                     (u_input == "y"))) {
                      done = true;
                      user_input_struct.user_selection = "Q";
                      return user_input_struct;
                  }
            }
    }

     
    ifstream file(infile_name);

    if (!file.is_open()) {
        cout << "Error opening file: " << infile_name  << endl;
        user_input_struct.user_selection = "Q";
        return user_input_struct;
    }
   
return user_input_struct;
    
}
//selection 2 is update the inventory from orders
Input_Values sel_2(inventory_table*& tbl, inventory_table_2*& tbl2, map<string, struct costruct> ord_tab, map<string, struct p_node> prod_tab)
{
 
    // local copy of Input_Values structure
    Input_Values user_struct;
  
    string infile = "";
    
    user_struct.user_selection = "0";
    user_struct = get_file_name();
    infile = user_struct.user_input_file_name;
    if (user_struct.user_selection == "Q")
    {
        return user_struct;
    }

    else
    {
        
        update_inventory_from_orders(ord_tab, prod_tab, tbl, tbl2);
        cout << "Successfully Changed Inventory Database" << endl << endl;
    }
   
    return user_struct;
}
//**** make sure user enters correct menu item for main menu
bool process_sel(string user_sel_p)
{

    bool valid = true;

    if ((user_sel_p.length() > 1)  || (user_sel_p.length() < 1))
    {
            valid = false;
            cout << "Please, enter selection 1, 2 or Q to quit" << endl;
    }
    else {
        if ((user_sel_p.substr(0,1) == "1") ||
            (user_sel_p.substr(0,1) == "2") ||
            (user_sel_p.substr(0,1) == "Q") ||
            (user_sel_p.substr(0,1) == "q"))
            {
                valid = true;
            } 
            else 
            {
                valid = false;
            }
        }
     
    return valid;
}
//********* output to screen main menu
  string menu()
{
    
    string menu_sel;
    
    cout << "MAIN MENU" << endl;
    cout << "1) Run Reports" << endl;
    cout << "2) Update Inventory from Orders" << endl;
    cout << "Enter menu selection (Q to quit): ";
    cin >> menu_sel;
    
    
    return menu_sel;
}

//*************** main driver for menu
bool prompt_user_selection(inventory_table*& tbl, inventory_table_2*& tbl2, map<string, struct reorder_detail> ro_tab, map<string, struct costruct> ord_tab, map<string, struct p_node> prod_tab)
{
  
    bool user_quit = false;
    bool done = false;
    Input_Values user_struct;

    
// define user selection string
    string user_sel;
   
// loop to validate user selection   
    while (!done)
    {
        user_sel = menu();
        done = process_sel(user_sel);
    }
    
  // user selected 1 - run reports - sel_1() offers reports menu      
    if (user_sel.length() == 1)
    {
        if (user_sel.substr(0,1) == "1")
        {
          sel_1(tbl,tbl2,ro_tab, ord_tab, prod_tab);
          return true;
    
        }
    }
  //if user selected 2  call function to get input file and update inventory
    if ((user_sel.length() == 1) && (user_sel.substr(0,1) == "2"))
    {
         cout << "Update Inventory Selected" << endl;
         user_struct = sel_2(tbl, tbl2, ord_tab, prod_tab);  
         return true;
    }

    if (user_sel.length() == 1) 
    {
        if ((user_sel.substr(0,1) == "Q") || (user_sel.substr(0,1) == "q"))
        {
          cout << "Quitting...: "<< endl;
          user_quit = true;
          return user_quit;
        }
    }

return false;
}

//**********************************************************
int main()
{
 
    inventory_table* tbl = init_inventory_table(initial_cap);
    inventory_table_2* tbl2 = init_inventory_table2(initial_cap);
    map<string, struct p_node> prod_tab;
    map<int, struct mstruct> man_tab;
    map<string, struct cstruct> cust_tab;
    map<string, struct costruct> ord_tab;
    map<string, struct reorder_detail> ro_tab;
    
  
    string invfile = "sbsinventory.csv";
    string product_file = "sbsproducts.csv";
    string man_file = "manufacturer.csv";
    string cust_info = "customer_info.csv";
    string cust_order = "orders_export.csv";
    string avg_sales = "average_daily_sales.csv";

     //create tables and maps
    update_inventory_table_from_file(invfile, tbl, tbl2);
    prod_tab = product_tab(product_file);
    man_tab = build_manufacturer_tab(man_file);
    cust_tab = build_cust_info_tab(cust_info);
    ord_tab = build_cust_ord_tab(cust_order); 
    update_inventory_from_orders(ord_tab, prod_tab, tbl, tbl2);

    ro_tab = build_reorder_tab(avg_sales, tbl, tbl2, man_tab, prod_tab);
    
    
    // define user selection string
    string user_sel;
    bool done = false;
       

// calls function that prompts user for selection and calls 
// all functions associated with selection 

    while (!done)
    {
        done = prompt_user_selection(tbl, tbl2, ro_tab, ord_tab, prod_tab);
    }
//backup tables and maps
    
 write_out_customer(cust_tab);
 write_out_inventory(tbl, tbl2);
 write_out_manufacturer(man_tab);
return 0; 
}

