#ifndef SIMULATION
#define SIMULATION

#include "User.hpp"
#include <typeinfo>
#include <map>
#include <fstream>
#include <sstream>
#include <iomanip>

class Simulation
{
public:
    static Simulation &getInstance()
    {
        static Simulation instance_;
        return instance_;
    };

    void addBuyer(int buyer_id, Buyer *b);
    void addSeller(int seller_id, Seller *s);
    void addProduct(int product_key, Product *p, Seller *s, int bidder_id, double bid);
    void addProduct(int product_key, Product *p, Seller *s, int bidder_id, double bid, std::string filename);

    void displayUsers();
    void displayProducts();
    void displayAvailProducts();

    Buyer *getBuyer(Buyer *b);
    Buyer *getBuyer(int buyer_id);
    Seller *getSeller(Seller *s);
    Seller *getSeller(int seller_id);
    Product *getProduct(int key);
    Buyer *getBuyerWinner(Product *p);

    void updateCSV(std::string filename, Buyer *user);
    void updateCSV(std::string filename, Seller *user);
    void updateCSV(std::string filename, Product *p, Seller *s);
    void updateCSV(std::string filename, Product *p, Seller *s, Buyer *b);

    void autoMakeBid(Product *p, Seller *s);
    void autoCloseBid(Product *p, Buyer *b);

    void wipeSimulation();

private:
    Simulation(){};

    std::vector<std::pair<int, Buyer *>> buyers_;
    std::vector<std::pair<int, Seller *>> sellers_;
    std::map<std::pair<int, double>, std::pair<int, int>> product_history_;
    std::vector<std::pair<int, std::pair<Product *, Seller *>>> products_;
};

// adds buyers into buyer vector
void Simulation::addBuyer(int buyer_id, Buyer *b)
{
    buyers_.push_back(std::make_pair(buyer_id, b));
    updateCSV("Users.csv", b);
}

// adds seller into seller vector
void Simulation::addSeller(int seller_id, Seller *s)
{
    sellers_.push_back(std::make_pair(seller_id, s));
    updateCSV("Users.csv", s);
}

//  adds products into products map with seller id and bid price
// this verson is for adding products into the vector when reading from csv
void Simulation::addProduct(int product_key, Product *p, Seller *s, int buyer_id, double bid)
{
    int i = 0;
    bool found = false;
    while (i < products_.size())
    {
        if (products_.at(i).second.first->getProductKey() == product_key)
        {
            found = true;
            products_.at(i).second.first->setBid(p->getBid());
            products_.at(i).second.first->setStatus(p->getStatus());
            if (p->getStatus() == 3)
            {
                Buyer *b = getBuyerWinner(p);
                b->addProducts(p, b->getUserID());
            }
            break;
        }
        ++i;
    }
    if (!found)
    {
        products_.push_back(std::make_pair(product_key, (std::make_pair(p, s))));
        s->addProducts(p, s->getUserID());
    }

    product_history_.insert(make_pair(std::make_pair(product_key, bid), std::make_pair(s->getUserID(), buyer_id)));
}

// this verson is for adding products into the vector when add new transactions from the user
void Simulation::addProduct(int product_key, Product *p, Seller *s, int buyer_id, double bid, std::string filename)
{
    int i = 0;
    bool found = false;
    while (i < products_.size())
    {
        if (products_.at(i).second.first == p)
        {
            found = true;
            products_.at(i).second.first->setBid(bid);
            products_.at(i).second.first->setStatus(p->getStatus());
            if (p->getStatus() == 3)
            {
                Buyer *b = getBuyerWinner(p);
                b->addProducts(p, b->getUserID());
            } 
            break;
        }
        ++i;
    }
    if (!found)
    {
        products_.push_back(std::make_pair(product_key, (std::make_pair(p, s))));
        s->addProducts(p, s->getUserID());
    }
    product_history_.insert(make_pair(std::make_pair(product_key, bid), std::make_pair(s->getUserID(), buyer_id)));
    if (bid == 0.0 || buyer_id == 0)
    {
        updateCSV(filename, p, s);
    }
    else
    {
        updateCSV(filename, p, s, getBuyer(buyer_id));
    }
}

// returns buyer from buyers_ vector given the buyer or the id
Buyer *Simulation::getBuyer(Buyer *b)
{
    for (int i = 0; i < buyers_.size(); i++)
    {
        if (buyers_.at(i).second == b)
        {
            return buyers_.at(i).second;
        }
    }
    return nullptr;
}

Buyer *Simulation::getBuyer(int buyer_id)
{
    for (int i = 0; i < buyers_.size(); i++)
    {
        if (buyers_.at(i).second->getUserID() == buyer_id)
        {
            return buyers_.at(i).second;
        }
    }
    return nullptr;
}

// returns seller from sellers_ vector given the seller or the id
Seller *Simulation::getSeller(Seller *s)
{
    for (int i = 0; i < sellers_.size(); i++)
    {
        if (sellers_.at(i).second == s)
        {
            return sellers_.at(i).second;
        }
    }
    return nullptr;
}

Seller *Simulation::getSeller(int seller_id)
{
    for (int i = 0; i < sellers_.size(); i++)
    {
        if (sellers_.at(i).second->getUserID() == seller_id)
        {
            return sellers_.at(i).second;
        }
    }
    return nullptr;
}

// returns product from products_ vector given the product id
Product *Simulation::getProduct(int key)
{
    bool product_found = false;
    for (int i = 0; i < products_.size(); i++)
    {
        if (products_.at(i).second.first->getProductKey() == key)
        {
            product_found = true;
            return products_.at(i).second.first;
        }
    }
    if (!product_found)
    {
        std::cerr << "error: product not found." << std::endl;
        return nullptr;
    }
    return nullptr;
}

// returns finds the winner of a closed bid
Buyer *Simulation::getBuyerWinner(Product *p)
{
    Buyer *bidder;
    int bidder_id = 0;
    // check if product has been sold
    for (int i = 0; i < products_.size(); i++)
    {
        if (products_.at(i).second.first->getProductKey() == p->getProductKey())
        {
            if (products_.at(i).second.first->getStatus() == 1)
            {
                return nullptr;
            }
        }
    }

    for (auto itr = product_history_.begin(); itr != product_history_.end(); itr++) 
    {
        if (itr->first.first == p->getProductKey()) 
        {
            bidder_id = itr->second.second;
        }
    }
    
    return getBuyer(bidder_id);
}
// displays both sellers and users
void Simulation::displayUsers()
{
    if (buyers_.empty())
    {
        std::cerr << "No buyers found." << std::endl;
    }
    else
    {
        for (auto buyer : buyers_)
        {
            std::cout << "Buyer ID: " << buyer.first << ", Buyer Name: " << buyer.second->getName() << std::endl;
        }
    }

    if (sellers_.empty())
    {
        std::cerr << "No sellers found." << std::endl;
    }
    else
    {
        for (auto seller : sellers_)
        {
            std::cout << "Seller ID: " << seller.first << ", Seller Name: " << seller.second->getName() << std::endl;
        }
    }
}

// displays all products
void Simulation::displayProducts()
{
    if (products_.empty())
    {
        std::cerr << "No buyers found." << std::endl;
    }
    else
    {
        for (auto product : products_)
        {
            std::cout << "Product ID: " << product.first << "Product Name: " << product.second.first->getProductName() << " sold by " << product.second.second->getName() << std::endl;
        }
    }
}

// displays available products
void Simulation::displayAvailProducts()
{
    if (products_.empty())
    {
        std::cerr << "No products found." << std::endl;
    }
    else
    {
        for (auto product : products_)
        {
            if (product.second.first->getStatus() == 1)
            {
                std::cout << "Product ID: " << product.first << " | Product Name: " << product.second.first->getProductName()
                          << " | sold by " << product.second.second->getName() << " | current highest bid: " << product.second.first->getBid() << std::endl;
            }
        }
    } std::cout << std::endl;
}

//  updates CSV with new information according to the type of information provided
//  updates buyer information
void Simulation::updateCSV(std::string filename, Buyer *user)
{
    std::fstream file(filename);
    std::vector<int> buyer_ids;
    bool found_user = false;
    std::ostringstream balance_str;
    balance_str << std::fixed << std::setprecision(2) << user->getBalance();
    std::string new_line = std::to_string(user->getUserID()) + "," + user->getName() + "," + user->getPhoneNumber() + "," + user->getAddress() + "," + balance_str.str();

    if (file.is_open())
    {
        std::string line;

        // Read each line of the file and parse the data
        while (getline(file, line))
        {
            std::stringstream ss(line);
            std::string id, buyer_details;
            int buyer_id;

            getline(ss, id, ',');
            getline(ss, buyer_details, ',');
            buyer_id = stoi(id);

            if (buyer_id == user->getUserID())
            {
                found_user = true;
                std::string replacement_str = std::to_string(user->getUserID()) + "," + user->getName() + "," + user->getPhoneNumber() + "," + user->getAddress() + "," + std::to_string(user->getBalance());
                line = replacement_str;
                break;
            }
        }
        if (!found_user)
        {
            std::string new_line = std::to_string(user->getUserID()) + "," + user->getName() + "," + user->getPhoneNumber() + "," + user->getAddress() + ",";
            file.seekp(0, std::ios::end);
            file << new_line << std::fixed << std::setprecision(2) << std::to_string(user->getBalance()) << std::endl;
        }

        file.close();
    }
    else
    {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}

// updates seller information
void Simulation::updateCSV(std::string filename, Seller *user)
{
    std::fstream file(filename);
    std::vector<int> buyer_ids;
    bool found_user = false;

    if (file.is_open())
    {
        std::string line;

        // Read each line of the file and parse the data
        while (getline(file, line))
        {
            std::stringstream ss(line);
            std::string id, buyer_details;
            int buyer_id;

            getline(ss, id, ',');
            getline(ss, buyer_details, ',');
            buyer_id = stoi(id);

            if (buyer_id == user->getUserID())
            {
                found_user = true;
                std::string replacement_str = std::to_string(user->getUserID()) + "," + user->getName() + "," + user->getPhoneNumber() + "," + user->getAddress() + "," + std::to_string(user->getBalance());
                line = replacement_str;
                break;
            }
        }
        if (!found_user) {
            std::string new_line = std::to_string(user->getUserID()) + "," + user->getName() + "," + user->getPhoneNumber() 
                + "," + user->getAddress() + ",";
            file.seekp(0, std::ios::end);
            file << new_line << std::fixed << std::setprecision(2) << std::to_string(user->getBalance()) << std::endl;
        }

        file.close();
    }
    else
    {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}

// updates product information where the product is newly opened
void Simulation::updateCSV(std::string filename, Product *p, Seller *s)
{
    std::ofstream fout;
    double bid_amount = 0.0;
    std::string product_type = "0";

    if (p->getCategory() == ProductCategory::Clothing)
    {
        product_type = "1";
    }
    else if (p->getCategory() == ProductCategory::Electronics)
    {
        product_type = "2";
    }
    else if (p->getCategory() == ProductCategory::Toys)
    {
        product_type = "3";
    }
    else if (p->getCategory() == ProductCategory::SportingGoods)
    {
        product_type = "4";
    }
    else if (p->getCategory() == ProductCategory::Furniture)
    {
        product_type = "5";
    }
    else
    {
        product_type = "0";
    }

    fout.open(filename, std::ios::app);
    if (fout.is_open()) {
        fout << std::endl << std::to_string(p->getProductKey()) + "," + p->getProductName() + "," << std::fixed << std::setprecision(2) << p->getPrice() << "," << std::fixed << std::setprecision(2) << bid_amount << 
            "," + std::to_string(p->getQuality()) + "," + p->getBrand() + "," + std::to_string(p->getStatus()) + "," + std::to_string(0) + "," + std::to_string(s->getUserID()) + "," + std::to_string(0) + "," + product_type;

        fout.close();
    }
    else
    {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}

// updates product information 
void Simulation::updateCSV(std::string filename, Product *p, Seller *s, Buyer *b)
{
    std::ofstream fout;
    double bid_amount = 0.0;
    std::vector<std::pair<Product *, double>> temp = b->getPlacedBids();
    std::vector<Product *> temp_products = b->getProduct();
    int buyer_product = 0;
    std::string product_type = "0";

    for (auto it = temp.rbegin(); it != temp.rend(); ++it)
    {
        if (it->first == p)
        {
            bid_amount = it->second;
            break;
        }
    }

    for (int i = 0; i < temp_products.size(); i++)
    {
        if (temp_products.at(i)->getProductKey() == p->getProductKey())
        {
            buyer_product = b->getUserID();
            break;
        }
    }

    if (p->getCategory() == ProductCategory::Clothing)
    {
        product_type = "1";
    }
    else if (p->getCategory() == ProductCategory::Electronics)
    {
        product_type = "2";
    }
    else if (p->getCategory() == ProductCategory::Toys)
    {
        product_type = "3";
    }
    else if (p->getCategory() == ProductCategory::SportingGoods)
    {
        product_type = "4";
    }
    else if (p->getCategory() == ProductCategory::Furniture)
    {
        product_type = "5";
    }
    else
    {
        product_type = "0";
    }

    fout.open(filename, std::ios::app);
    if (fout.is_open()) {
        fout << std::endl << std::to_string(p->getProductKey()) + "," + p->getProductName() + "," << std::fixed << std::setprecision(2) << p->getPrice() << "," << std::fixed << std::setprecision(2) << bid_amount << 
            std::to_string(p->getQuality()) + "," + p->getBrand() + "," + std::to_string(p->getStatus()) + "," + std::to_string(b->getUserID()) + "," + std::to_string(s->getUserID()) + "," + std::to_string(buyer_product) + "," + product_type;
        fout.close();
    }
    else
    {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}

// simulation makes a bid that is 100 more than highest
// bid on a given product
void Simulation::autoMakeBid(Product *p, Seller *s)
{
    int random = rand() % buyers_.size();
    Buyer *temp = buyers_.at(random).second;
    double bidding_amount = p->getBid() + 100.00;

    temp->placeBid(p, bidding_amount);
    updateCSV("Product_Bid.csv", p, s, temp);
}

// simulation closes the bidding on a given product
// gives product to buyer if highest bid
void Simulation::autoCloseBid(Product *p, Buyer *b)
{
    Seller *temp;
    for (int i = 0; i < products_.size(); i++)
    {
        if (products_.at(i).second.first == p)
        {
            temp = products_.at(i).second.second;
            addProduct(p->getProductKey(), p, temp, b->getUserID(), p->getBid(), "Product_Bid.csv");
            temp->closeBid(p);
            updateCSV("Product_Bid.csv", p, temp, b);
            break;
        }
    }

    if (b == getBuyerWinner(p))
    {
        b->buyProduct(p, temp);
        updateCSV("Product_Bid.csv", p, temp, b);

    }
    else
    {
        std::cout << "sorry! you did not win this bid" << std::endl;
    }
}

// delete simulation
void Simulation::wipeSimulation()
{
    for (auto &seller : sellers_)
    {
        delete seller.second;
    }
    for (auto &buyer : buyers_)
    {
        delete buyer.second;
    }

    sellers_.clear();
    buyers_.clear();
    products_.clear();
    product_history_.clear();
}

#endif