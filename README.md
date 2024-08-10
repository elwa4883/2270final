# 2270final
Inventory Database

PROJECT GOAL
For this project I wanted to create a type of “relational database” to keep track of the inventory for a business that my husband started, and to create useful business reports from the data.

The company sells several products that adapt to electric skateboards. Each product is assembled from many smaller components that are manufactured from different companies around the world. When a product is sold, I wanted a way to subtract each separate component from the inventory database. Also, each company that we purchase from has a different lead time for delivery. I wanted a way to track when we would need to purchase each individual component so that we will not have an “out of stock” situation.
This is an example schematic from one of the products:

DATA STRUCTURES
The main database is the Individual Component Database:
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

I used the cuckoo hash method to place the individual components into 2 tables.  Each table is keyed with a separate hash method. For the first table, I chose a midsquare hash because it is said to have less collisions. Because my inventory keys started at 1, I actually multiplied the inventory key by a prime number that I randomly chose, so that the middle 4 digits would not be packed with zeroes. The second table is hashed by the djb2 hash function we used in class.
I chose the cuckoo hash method because it has a O(1) for component searches. The component is in either 1 or the other table always with the same hash. 
The cuckoo hash runs by using the key to calculate a hash for the first table and if the bucket is occupied, the new data takes the spot and displaces the current resident. The current residents key is then used to create a hash code for the 2nd table. If this bucket is occupied, the prior displaced occupant takes his home in the new table and that occupant is displaced and is then rehashed to the first table. This continues until all components are placed. For a “real world” implantation, I would resize the 2 tables every time I ran out of room. For this implantation, because I ran out of time, I used the fix iteration approach for the loop. Because my data is not extremely large, it worked.

To test my algorithm for adding the components to the hash tables, I decreased the size of my tables so that I would have collisions in both tables, to make sure my loop for moving around inventory components worked in all scenarios.

For the other “databases”, I made a series of maps.
The Product Table  (map<string, struct p_node> prod_tab) maps the larger products to their individual components.
The Manufacturer Table (map<int, struct mstruct> man_tab) holds the manufacturer contact info, shipping method and lead times.
The Customer Table (map<string, struct cstruct> cust_tab) has information about the individual customers.
The Order Table (map<string, struct costruct> ord_tab) is made of daily orders and is used to decrement ordered items from the inventory database.

Since I don’t have a downloaded database system, each run of the program – rebuilds all of the data tables and once all processes are completed – generates new files to use in the next run of the program. In a “real-life” scenario, I would want to do this with an actual relational database system. 
For this project, however, I thought it would be more fun to create everything from scratch.

THE ALGORITHMS
The code is initiated from ./run_app
The driver of the code is main.cpp
main.cpp – executes functions from inventory.cpp and relies on inventory.h

Once ./run_app is executed, main.cpp creates all the necessary databases in the from hard-coded input file names. I’ve uploaded the files to git hub and they are as follows:

    string inventory file = "sbsinventory.csv";
    string product file = "sbsproducts.csv";
    string manufacturing info file= "manufacturer.csv";
    string customer info file = "customer_info.csv";
    string customer order file = "orders_export.csv";
    string average daily sales file = "average_daily_sales.csv";

Once the “databases” / tables are built, the user is prompted with a menu:
    MAIN MENU
    1) Run Reports
    2) Update Inventory from Orders
    Enter menu selection (Q to quit): 

If the user selects 1, they get the following submenu:
    Reports Menu
    1) Create Inventory Report
    2) Create Report of Items to Reorder
    3) Return to Main Menu
    Enter menu selection (Q to quit): 

1)	Create Inventory Report – prints a copy of current inventory database.
2)	Create Report of Items to Reorder – prints a report that shows the following:

Which will come out in a file called reorder.info.csv
The algorithm to calculate the number of days until reorder is as follows:
The average daily sales is a file created by the web server that we use.  It is a rolling average of daily sales of each product.
I calculated the amount of the individual components that would be sold between the time we placed an order with a manufacturer and the time we actually received the components.
sold_during_transit =  (rd.avg_day_sales * rd.lead_time);
I added a 10% cushion to keep track of the minimum quantity we need in stock at all times
   	minquant = (1.10 * sold_during_transit);
Then I calculated the days until we need to replenish with the following
     	days_from_min = ((rd.in_stock - minquant)/rd.avg_day_sales);

If the user selects 2 from the main menu
Update Inventory from Orders
The user is prompted to input the name of a .csv file and if the file is appropriately named (.csv) and exists, the inventory database is updated from the order file.
The example file that I uploaded for this is orders_export.csv
Please, input this file name to sample this function.

Once the user has selected quit of the process requested is completed, the “databases” are copied into output .csv files to be used to reload the databases the next day. 

Things for the future:
I did not correctly anticipate the amount of time this project would take. In the future, I would like to do this same type of inventory system but with a permanent database. Also, I would like to test for every scenario that I could anticipate. This is definitely in the creation phase. Also, I would like to use the data for many other things and incorporate more reports that we get from the web host to help us run the business.

