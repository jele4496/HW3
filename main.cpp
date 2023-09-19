#include "User.hpp"
#include "Product.hpp"
#include "Simulation.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

int main()
{
    ProductFactory factory;
    // calls simulation instance
    static Simulation &sim = Simulation::getInstance();

    // read users.csv file and create users
    std::fstream user_file("Users.csv");

    if (!user_file.is_open())
    {
        std::cerr << "Error opening user file." << std::endl;
        return 1;
    }

    std::string line;

    while (getline(user_file, line))
    {
        std::stringstream ss_user(line);
        std::string user_info;
        std::vector<std::string> fields;

        while (getline(ss_user, user_info, ','))
        {
            fields.push_back(user_info);
        }

        int user_id = stoi(fields[0]);
        std::string user_name = fields[1], phone = fields[2], address = fields[3];
        double balance = stod(fields[4]);

        // if (ss.fail())
        // {
        //     std::cerr << "Error reading user file." << std::endl;
        //     ss.clear();
        //     std::string dummy;
        //     ss >> dummy;
        //     continue;
        // }

        if (user_name.size() < 1 || user_name.size() > 20 || phone.size() != 10)
        {
            std::cerr << "Invalid user information." << std::endl;
            return 1;
        }

        if (balance >= 0 && balance <= 1000000)
        {
            if (user_id < 500)
            {
                // std::cout << "Adding Seller." << std::endl;
                Seller *user = new Seller(user_id, user_name, phone, address, balance);
                sim.addSeller(user_id, user);
            }
            else
            {
                // std::cout << "Adding Buyer." << std::endl;
                Buyer *user = new Buyer(user_id, user_name, phone, address, balance);
                sim.addBuyer(user_id, user);
            }
        }
        else
        {
            std::cerr << "Invalid balance." << std::endl;
            return 1;
        }
    }

    user_file.close();

    // read products.csv file and create products
    std::ifstream product_file("Product_Bid.csv");

    if (!product_file.is_open())
    {
        std::cerr << "Error opening product file." << std::endl;
        return 1;
    }

    while (getline(product_file, line))
    {
        std::stringstream ss(line);
        std::string info;
        std::vector<std::string> fields;

        while (getline(ss, info, ','))
        {
            fields.push_back(info);
        }

        int product_id = stoi(fields[0]), quality = stoi(fields[4]), status = stoi(fields[6]),
            bidder_id = stoi(fields[7]), seller_id = stoi(fields[8]), buyer_id = stoi(fields[9]), product_type = stoi(fields[10]);
        std::string product_name = fields[1], brand = fields[5];
        double base_price = stod(fields[2]), bid_price = stod(fields[3]);

        // ss should be seperated into product_id_, product_name_, base_price, bid_price_, quality, brand, status, bidder_id_, seller_id_, buyer_id_, product_type(int)
        Product *product = factory.make_product(product_type, product_id, product_name, base_price, bid_price, quality, brand, status);
        Seller *product_seller = sim.getSeller(seller_id);
        Buyer *bidder = sim.getBuyer(bidder_id);
        sim.addProduct(product_id, product, product_seller, bidder->getUserID(), bid_price);

        //  if (sim.getProduct(product_id) != product) {
        //      std::cout << "adding new product." << std::endl;
        //      sim.addProduct(product_id, product, product_seller, nullptr, base_price);
        //  }
    }

    product_file.close();

    // prompt user for their choice (Seller = 1, Buyer = 2)
    // prompt for their name, phone number, address, and starting balance
    int choice;
    std::string username, phone, address;
    double balance;
    std::cout << "Are you a Seller or a Buyer? Enter 1 for Seller, 2 for Buyer: ";
    std::cin >> choice;
    if (choice == 1)
    {
        std::cout << "Enter your name: ";
        std::cin >> username;
        std::cout << "Enter your phone number: ";
        std::cin >> phone;
        std::cout << "Enter your address: ";
        std::cin.ignore();
        getline(std::cin, address);
        std::cout << "Enter your starting balance: ";
        std::cin >> balance;
        if (balance < 0 || balance > 1000000)
        {
            std::cerr << "Invalid balance." << std::endl;
            return 1;
        }
        Seller *seller = new Seller(username, phone, address, balance);
        sim.addSeller(seller->getUserID(), seller);

        // prompt seller to list a product

        int product_choice, quality, status;
        std::string name, brand;
        double price;

        std::cout << "Enter the product category (1. Clothing, 2. Electronics, 3. Toys, 4. Sporting Goods, 5. Furniture): ";
        std::cin >> product_choice;

        std::cout << "Enter the product name: ";
        std::cin.ignore();
        getline(std::cin, name);

        std::cout << "Enter the product price: ";
        std::cin >> price;

        std::cout << "Enter the product quality (1. New, 2. Used-Very Good, 3. Used-Good, 4. Used-Okay): ";
        std::cin >> quality;
        if (quality < 1 || quality > 4)
        {
            std::cerr << "Invalid quality." << std::endl;
            return 1;
        }

        if (product_choice != 0)
        {
            std::cout << "Enter the product brand: ";
            std::cin.ignore();
            getline(std::cin, brand);
        }
        std::cout << "Enter the product status (1. Open Bid, 2. Closed Bid-Unsold, 3. Closed Bid-Sold): ";
        std::cin >> status;
        if (status < 1 || status > 3)
        {
            std::cerr << "Invalid status." << std::endl;
            return 1;
        }
        Product *product = factory.make_product(product_choice, name, price, quality, brand, status);
        // add product for sale to seller's vector
        sim.addProduct(product->getProductKey(), product, seller, 0, 0.00, "Product_Bid.csv");
        sim.autoMakeBid(product, seller);

        // TO DO : prompt user to close bid
        // std::cout << "Press 'c' to close the bid, or any other key to exit: ";
        // char close_bid_choice;
        // std::cin >> close_bid_choice;
        // if (close_bid_choice == 'c')
        // {
        //     sim.autoCloseBid(product, seller);
        // }
    }
    else if (choice == 2)
    {
        std::cout << "Enter your name: ";
        std::cin >> username;
        std::cout << "Enter your phone number: ";
        std::cin >> phone;
        std::cout << "Enter your address: ";
        std::cin.ignore();
        getline(std::cin, address);
        std::cout << "Enter your starting balance: ";
        std::cin >> balance;
        if (balance < 0 || balance > 1000000)
        {
            std::cerr << "Invalid balance." << std::endl;
            return 1;
        }
        Buyer *buyer = new Buyer(username, phone, address, balance);
        sim.addBuyer(buyer->getUserID(), buyer);
        std::cout << "created acoount for " << username << " with ID " << buyer->getUserID() << std::endl
                  << std::endl;

        // prompt buyer to place a bid
        sim.displayAvailProducts();
        std::cout << "Enter the ID of the product you want to bid on: ";
        int product_id;
        std::cin >> product_id;
        Product *chosen_product = sim.getProduct(product_id);
        if (chosen_product->getProductKey() == product_id)
        {
            std::cout << "Enter your bid: ";
            double bid;
            std::cin >> bid;
            if (bid > chosen_product->getBid())
            {
                std::cout << "product's old bid: " << chosen_product->getBid() << std::endl;
                buyer->placeBid(chosen_product, bid);
                chosen_product->setBid(bid);
                std::cout << "Your bid is : " << bid << std::endl;
                std::cout << "product's new bid: " << chosen_product->getBid() << std::endl;
                sim.autoCloseBid(chosen_product, buyer);
            }
            else
            {
                std::cout << "Your bid was too low." << std::endl;
                chosen_product->setStatus(2);
            }
        }
    }
    else
    {
        std::cerr << "Invalid choice." << std::endl;
        return 1;
    }
    // delete simulation at the end
    sim.wipeSimulation();

    return 0;
}