#ifndef USER
#define USER

#include "Product.hpp"

class User
{
public:
    User(std::string name, std::string phone_number, std::string address, double balance) : name_(name), phone_number_(phone_number), address_(address), balance_(balance){};
    User(int user_id, std::string name, std::string phone_number, std::string address, double balance) : user_id_(user_id), name_(name), phone_number_(phone_number), address_(address), balance_(balance){};

    std::string getName() const { return name_; };
    std::string getPhoneNumber() const { return phone_number_; };
    std::string getAddress() const { return address_; };
    double getBalance() { return balance_; };
    int getUserID() const { return user_id_; };

    void setName(std::string name) { name_ = name; };
    void setPhoneNumber(int phone_number) { phone_number_ = phone_number; };
    void setAddress(std::string address) { address_ = address; };
    void setBalance(double balance) { balance_ = balance; };
    void addProducts(Product *p, int user_id);

    bool canMessage(User target);
    std::string getMessage(User target);
    void appendMessage(User target, std::string message);

    std::vector<Product *> getProduct() { return products_; };

protected:
    int user_id_ = 0;
    std::string name_;
    std::string phone_number_;
    std::string address_;
    double balance_;
    std::vector<Product *> products_;
};

void User::addProducts(Product *p, int user_id)
{
    if (user_id == user_id_)
    {
        products_.push_back(p);
    }
}

bool User::canMessage(User target)
{
    return false;
}

std::string User::getMessage(User target)
{
    return "Hello World!";
}

void User::appendMessage(User target, std::string message)
{
    // Find the index of the target user in the user's message history
    // int index = -1;
    // for (int i = 0; i < message.size(); i++)
    // {
    //     if (message[i].first == target.getUserID())
    //     {
    //         index = i;
    //         break;
    //     }
    // }

    // // If the target user is not in the user's message history, add them
    // if (index == -1)
    // {
    //     messages.push_back(std::make_pair(target.getUserID(), std::vector<std::string>()));
    //     index = messages_.size() - 1;
    // }

    // // Add the message to the target user's message history
    // messages[index].second.push_back(message);
}

////////////////////////////////////////////////////////////
/////////////////////// SELLER CLASS ///////////////////////
////////////////////////////////////////////////////////////
static int seller_id_ = 100;
class Seller : public User
{
public:
    Seller(std::string name, std::string phone_number, std::string address, double balance) : User(name, phone_number, address, balance) { user_id_ = ++seller_id_; };
    Seller(int user_id, std::string name, std::string phone_number, std::string address, double balance) : User(user_id, name, phone_number, address, balance){};

    void setBalance(double balance)
    {
        if (balance > balance_)
            setBalance(balance);
    };
    bool canMessage(User target);
    std::string getMessage(User target);

    void addProductForSale(Product *p) { products_.push_back(p); };
    void viewSimilarProducts(Product *p);
    void closeBid(Product *p);
    void reopenBid(Product *p);

private:
};

// TO DO : change to where the product must be closed (status = 2) and Product is in the Buyer's product vector
bool Seller::canMessage(User target)
{
    for (auto it = products_.begin(); it != products_.end(); ++it)
    {
        for (auto it_ = *target.getProduct().begin(); it_ != *target.getProduct().end(); ++it)
        {
            if (*it == it_)
            {
                if (it_->getStatus() == 2)
                {
                    return true;
                }
                break;
            }
        }
    }
    return false;
}

// TO DO : Change to get messages from csv? or whatever we are using to hold the messages
std::string Seller::getMessage(User target)
{
    return "in Seller::getMessage()";
}

// TO DO : Change to show products of similar products on market with status = 3 and type the same as their product
void Seller::viewSimilarProducts(Product *p)
{
    std::cout << "show similar products" << std::endl;
}

void Seller::closeBid(Product *p)
{
    for (int i = 0; i < products_.size(); i++)
    {
        Product *temp = products_.at(i);
        if (p->getProductKey() == temp->getProductKey())
        {
            std::cout << "closing bid on " << p->getProductName() << std::endl;
            products_.at(i)->setStatus(2);
            return;
        }
    }
    std::cout << "error: product not found" << std::endl;
    // TO DO:
    // update csv in order to hold seller, buyer, and product key
}

void Seller::reopenBid(Product *p)
{
    for (int i = 0; i < products_.size(); i++)
    {
        Product *temp = products_.at(i);
        if (p->getProductKey() == temp->getProductKey())
        {
            std::cout << "found " << p->getProductName() << std::endl;
            return;
        }
    }
    std::cout << "error: product not found" << std::endl;
}

///////////////////////////////////////////////////////////
/////////////////////// BUYER CLASS ///////////////////////
///////////////////////////////////////////////////////////
static int buyer_id_ = 500;
class Buyer : public User
{
public:
    Buyer(std::string name, std::string phone_number, std::string address, double balance) : User(name, phone_number, address, balance) { user_id_ = ++buyer_id_; };
    Buyer(int user_id, std::string name, std::string phone_number, std::string address, double balance) : User(user_id, name, phone_number, address, balance){};

    void setBalance(double balance)
    {
        if (balance < balance_)
            setBalance(balance);
    };
    bool canMessage(User target);
    std::string getMessage(User target);
    std::vector<std::pair<Product *, double>> getPlacedBids() { return placed_bids_; };

    void placeBid(Product *p, double amount);
    void buyProduct(Product *product, Seller *seller);

private:
    std::vector<std::pair<Product *, double>> placed_bids_;
};

// TO DO : change to where the product must be closed (status = 2) and Product is in the Seller's product vector
bool Buyer::canMessage(User target)
{
    for (auto it = products_.begin(); it != products_.end(); ++it)
    {
        for (auto it_ = *target.getProduct().begin(); it_ != *target.getProduct().end(); ++it)
        {
            if (*it == it_)
            {
                if (it_->getStatus() == 2)
                {
                    return true;
                }
                break;
            }
        }
    }
    return false;
}

// TO DO : Change to get messages from csv? or whatever we are using to hold the messages
std::string Buyer::getMessage(User target)
{
    return "in Buyer::getMessage()";
}

void Buyer::placeBid(Product *p, double amount)
{
    placed_bids_.push_back(std::make_pair(p, amount));
}

void Buyer::buyProduct(Product *product, Seller *seller)
{
    if (product->getPrice() > balance_)
    {
        std::cout << "Sorry, you don't have enough balance to buy this product." << std::endl;
        product->setStatus(2);
        return;
    }

    if (seller->getProduct().size() == 0)
    {
        std::cout << "Sorry, the seller has no products for sale." << std::endl;
        return;
    }

    bool found = false;
    for (int i = 0; i < seller->getProduct().size(); i++)
    {
        if (seller->getProduct().at(i)->getProductKey() == product->getProductKey())
        {
            found = true;
            break;
        }
    }

    if (!found)
    {
        std::cout << "Sorry, the seller does not have this product for sale." << std::endl;
        return;
    }
    balance_ -= product->getPrice();
    std::cout << "Congratulations, you have successfully bought " << product->getProductName()
              << " from " << seller->getName() << " for $" << product->getBid() << std::endl;
    product->setStatus(3);
    addProducts(product, user_id_);
}

#endif