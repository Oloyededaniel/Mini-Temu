#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <iomanip>

using namespace std;

// Class representing a product review
class Review {
public:
    string username;  // Username of the reviewer
    string comment;   // Review text
    int rating;       // Rating from 1 to 5 stars
    string date;      // Review date

    // Constructor to initialize Review object
    Review(string username, string comment, int rating) 
        : username(username), comment(comment), rating(rating) {
        date = getCurrentDate();  // Set current date on creation of the review
    }

private:
    // Get the current date as a string
    string getCurrentDate() {
        time_t now = time(0);
        string date = ctime(&now);
        return date.substr(0, date.length() - 1);  // Remove newline character
    }
};

// Class representing a product in the system
class Product {
public:
    string name;          // Name of the product
    double price;        // Price of the product
    string category;     // Category of the product
    int quantity;        // Available quantity of the product
    vector<Review> reviews; // List of reviews for the product
    double averageRating; // Average rating calculated from reviews
    string sellerName;   // Name of the seller
    bool onSale;         // Status if product is currently on sale
    double salePrice;    // Sale price if on sale

    // Constructor to create a Product object
    Product(string name, double price, string category, int quantity, string seller) 
        : name(name), price(price), category(category), quantity(quantity), 
          sellerName(seller), averageRating(0.0), onSale(false), salePrice(price) {}

    // Method to add a review to the product
    void addReview(const string& username, const string& comment, int rating) {
        reviews.push_back(Review(username, comment, rating)); // Add review
        updateAverageRating(); // Update the average rating after adding a new review
    }

    // Set a sale price for the product
    void setSalePrice(double discount) {
        onSale = true; // Set onSale status to true
        salePrice = price * (1 - discount / 100); // Calculate the new sale price
    }

    // End the sale and revert to the original price
    void endSale() {
        onSale = false; // Set onSale status to false
        salePrice = price; // Revert sale price back to the original price
    }

private:
    // Update the average rating of the product based on existing reviews
    void updateAverageRating() {
        if (reviews.empty()) return; // Exit if there are no reviews        
        double sum = 0; // Variable to hold the sum of the ratings
        for (const auto& review : reviews) {
            sum += review.rating; // Accumulate the rating values
        }
        averageRating = sum / reviews.size(); // Calculate and set the average rating
    }
};

// Class responsible for managing a collection of products
class ProductManager {
private:
    vector<Product> products; // Container for all products

public:
    // Method to add a new product to the collection
    void addProduct(const string& name, double price, const string& category, int quantity, const string& seller) {
        products.emplace_back(name, price, category, quantity, seller); // Create and add a new product
        cout << "Product '" << name << "' added successfully with " << quantity << " units.\n";
    }

    // Display a list of all available products
    void displayAllProducts() const {
        if (products.empty()) {
            cout << "No products available.\n"; // Notify if no products exist
            return;
        }
        cout << "Available Products:\n";
        for (const auto& product : products) {
            cout << "- " << product.name << " ($" << product.price << ") [" << product.category << "] - " 
                 << product.quantity << " units available";
            if (product.onSale) {
                cout << " (ON SALE: $" << product.salePrice << ")"; // Indicate if the product is on sale
            }
            cout << "\n";
        }
    }

    // Display detailed information about a specific product
    void displayProductDetails(const string& productName) const {
        // Find product by name
        auto it = find_if(products.begin(), products.end(), 
            [&productName](const Product& p) { return p.name == productName; });

        if (it != products.end()) { // If product was found
            cout << "\n=== Product Details ===\n";
            cout << "Name: " << it->name << "\n";
            cout << "Category: " << it->category << "\n";
            cout << "Seller: " << it->sellerName << "\n";
            cout << "Regular Price: $" << it->price << "\n";
            if (it->onSale) { // Show sale price if applicable
                cout << "ON SALE: $" << it->salePrice << " (" 
                     << (100 * (1 - it->salePrice / it->price)) << "% off!)\n";
            }
            cout << "Quantity Available: " << it->quantity << "\n";
            cout << "Average Rating: " << fixed << setprecision(1) << it->averageRating << "/5.0\n"; // Format average rating

            // Display customer reviews
            if (!it->reviews.empty()) {
                cout << "\nCustomer Reviews:\n";
                for (const auto& review : it->reviews) {
                    cout << "★" << review.rating << " - " << review.username << " (" << review.date << ")\n";
                    cout << "\"" << review.comment << "\"\n\n"; // Display each review's comment and rating
                }
            }
        } else {
            cout << "Product not found.\n"; // Notify if product not found
        }
    }

    // Display inventory details of all products
    void displayInventory() const {
        if (products.empty()) {
            cout << "No products available in inventory.\n"; // Notify if inventory is empty
            return;
        }

        cout << "\n=== Inventory Details ===\n";
        for (const auto& product : products) {
            cout << "Product: " << product.name << "\n";
            cout << "Category: " << product.category << "\n";
            cout << "Price: $" << product.price << "\n";
            if (product.onSale) {
                cout << "Sale Price: $" << product.salePrice << " (ON SALE)\n"; // Indicate sale price
            }
            cout << "Quantity Available: " << product.quantity << "\n";
            cout << "Average Rating: " << fixed << setprecision(1) << product.averageRating << "/5.0\n"; // Display average rating
            cout << "----------------------------\n";
        }
    }

    // Accessor to get the list of products
    vector<Product>& getProducts() {
        return products;
    }

    // Update the quantity of a product in stock
    bool updateProductQuantity(const string& productName, int quantityToReduce) {
        auto it = find_if(products.begin(), products.end(), 
            [&productName](const Product& p) { return p.name == productName; });

        if (it != products.end() && it->quantity >= quantityToReduce) {
            it->quantity -= quantityToReduce; // Reduce quantity
            return true; // Indicate success
        }
        return false; // Indicate failure to update
    }

    // Set a product on sale by applying a discount
    void setProductOnSale(const string& productName, double discountPercentage) {
        auto it = find_if(products.begin(), products.end(), 
            [&productName](const Product& p) { return p.name == productName; });

        if (it != products.end()) {
            it->setSalePrice(discountPercentage); // Apply sale price calculation
            cout << "Product '" << productName << "' is now on sale with " 
                 << discountPercentage << "% discount!\n";
        }
    }

    // Add a review to a specific product
    void addReviewToProduct(const string& productName, const string& username, const string& comment, int rating) {
        auto it = find_if(products.begin(), products.end(), 
            [&productName](const Product& p) { return p.name == productName; });

        if (it != products.end()) {
            it->addReview(username, comment, rating); // Add review to product
            cout << "Review added successfully!\n";
        } else {
            cout << "Product not found.\n"; // Notify if product not found
        }
    }
};

// Class representing an item in the shopping cart
class CartItem {
public:
    Product product; // The product associated with this cart item
    int quantity;    // The quantity of the product

    // Constructor to initialize a cart item
    CartItem(const Product& product, int quantity) : product(product), quantity(quantity) {}
};

// Class representing the shopping cart
class Cart {
private:
    vector<CartItem> items; // List of items in the cart

public:
    // Method to add a product to the cart
    void addItem(const Product& product, int quantity) {
        auto it = find_if(items.begin(), items.end(), [&product](const CartItem& item) {
            return item.product.name == product.name; // Check if the product is already in the cart
        });

        if (it != items.end()) {
            it->quantity += quantity; // Increase quantity if already exists in cart
        } else {
            items.emplace_back(product, quantity); // Add new item if not present
        }

        cout << "Added " << quantity << " of " << product.name << " to the cart.\n";
    }

    // View all items in the cart
    void viewCart() const {
        if (items.empty()) {
            cout << "Your cart is empty.\n"; // Notify if cart is empty
            return;
        }
        cout << "Your Cart:\n";
        double total = 0.0; // Variable to accumulate total cost
        for (const auto& item : items) {
            double subtotal = (item.product.onSale ? item.product.salePrice : item.product.price) * item.quantity; // Price based on sale status
            cout << "- " << item.product.name << " ($" << (item.product.onSale ? item.product.salePrice : item.product.price) << ") x " 
                 << item.quantity << " = $" << subtotal << "\n"; // Display item total
            total += subtotal; // Update total cost
        }
        cout << "Total: $" << total << "\n"; // Display total cost of cart
    }

    // Checkout and finalize the order
    vector<string> checkout() {
        if (items.empty()) {
            cout << "Your cart is empty. Add items before checking out.\n"; // Notify if cart is empty
            return {};
        }

        string address, city, postalCode;
        cout << "Enter delivery address: ";
        cin.ignore(); // Clear the input buffer
        getline(cin, address);
        cout << "Enter city: ";
        getline(cin, city);
        cout << "Enter postal code: ";
        getline(cin, postalCode);

        cout << "Order placed successfully! Delivery details:\n";
        cout << "Address: " << address << ", " << city << ", " << postalCode << "\n";
        
        // Store purchased products before clearing cart
        vector<string> purchasedProducts;
        for (const auto& item : items) {
            purchasedProducts.push_back(item.product.name); // Capture the name of purchased products
        }
        
        items.clear(); // Clear the cart after checkout
        return purchasedProducts; // Return the list of purchased products
    }
};

// Class representing a user in the system
class User {
protected:
    string username;   // User's username
    string password;   // User's password
    string role;       // User's role (customer/seller)

public:
    User(string username, string password, string role) : username(username), password(password), role(role) {}
    virtual ~User() = default; // Virtual destructor for proper cleanup in derived classes

    virtual string getRole() const { return role; } // Get the user's role
    string getUsername() const { return username; } // Get the username
    bool checkPassword(const string& pwd) const { return pwd == password; } // Check if the password matches
};

// Class representing a customer
class Customer : public User {
private:
    Cart cart; // Customer's shopping cart
    vector<string> purchasedProducts; // Products purchased by the customer

public:
    Customer(string username, string password) : User(username, password, "customer") {}

    // Add a product to the customer's cart
    void addToCart(const Product& product, int quantity) {
        cart.addItem(product, quantity); // Delegate to cart to handle adding items
    }

    // View the contents of the cart
    void viewCart() const {
        cart.viewCart(); // Delegate to cart for viewing contents
    }

    // Checkout the items in the cart
    void checkout() {
        vector<string> newPurchases = cart.checkout(); // Complete checkout
        purchasedProducts.insert(purchasedProducts.end(), newPurchases.begin(), newPurchases.end()); // Record purchased products
    }

    // Add a purchased product to the list
    void addPurchasedProduct(const string& productName) {
        purchasedProducts.push_back(productName); // Store the name of the purchased product
    }

    // Check if the customer has purchased a specific product
    bool hasPurchased(const string& productName) const {
        return find(purchasedProducts.begin(), purchasedProducts.end(), productName) != purchasedProducts.end(); // Check if product is in purchased list
    }
};

// Class representing a seller
class Seller : public User {
public:
    Seller(string username, string password) : User(username, password, "seller") {}

    // Method allowing sellers to add a product
    void addProduct(ProductManager& productManager) {
        string pname, pcategory, seller;
        double pprice;
        int pquantity;
        cout << "Enter product name: ";
        cin.ignore(); // Clear input buffer
        getline(cin, pname);
        cout << "Enter product price: ";
        cin >> pprice;
        cout << "Enter product category: ";
        cin.ignore(); // Clear input buffer
        getline(cin, pcategory);
        cout << "Enter product quantity: ";
        cin >> pquantity;
        cout << "Enter seller name: ";
        cin.ignore(); // Clear input buffer
        getline(cin, seller);
        productManager.addProduct(pname, pprice, pcategory, pquantity, seller); // Add product using the ProductManager
    }
};

// Class for managing users
class UserManager {
private:
    vector<User*> users; // List of registered users

public:
    ~UserManager() { // Destructor to clean up dynamically allocated Users
        for (User* user : users) {
            delete user;
        }
    }

    // Check if a username is already taken
    bool isUsernameTaken(const string& username) const {
        return any_of(users.begin(), users.end(),
            [&username](const User* user) { return user->getUsername() == username; }); // Search for matching username
    }

    // Register a new user (customer/seller)
    void registerUser(const string& username, const string& password, const string& role) {
        if (isUsernameTaken(username)) {
            cout << "Username already taken. Please choose a different username.\n"; // Notify if username is taken
            return;
        }

        // Create a new user based on their role
        if (role == "customer") {
            users.push_back(new Customer(username, password));
        } else if (role == "seller") {
            users.push_back(new Seller(username, password));
        }
        cout << "Registration successful for " << role << " '" << username << "'.\n"; // Confirm registration
    }

    // Method for user login
    bool login(const string& username, const string& password, ProductManager& productManager);
};

// Forward declaration for user menu display function
void displayUserMenu(User* user, ProductManager& productManager);

// Main function
int main() {
    ProductManager productManager; // Instance of the product manager
    UserManager userManager; // Instance of the user manager

    while (true) {
        cout << "\n=== Mini - Temu ===\n"; // Display application name
        cout << "1. Login\n";
        cout << "2. Sign Up\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        
        int choice; // Variable to hold user's menu choice
        cin >> choice;

        if (choice == 3) {
            cout << "Thank you for using our system. Goodbye!\n"; // Exit message
            break;
        }

        string username, password, role; // Variables to store user input
        if (choice == 1) {
            cout << "\n=== Login ===\n";
            cout << "Username: ";
            cin >> username;
            cout << "Password: ";
            cin >> password;

            if (!userManager.login(username, password, productManager)) {
                cout << "Invalid credentials. Please try again.\n"; // Notify on invalid login
            }
        }
        else if (choice == 2) {
            cout << "\n=== Sign Up ===\n";
            cout << "Username: ";
            cin >> username;
            cout << "Password: ";
            cin >> password;
            cout << "Role (customer/seller): ";
            cin >> role;

            // Normalize role input to lowercase for consistency
            transform(role.begin(), role.end(), role.begin(), ::tolower);
            
            if (role != "customer" && role != "seller") {
                cout << "Invalid role. Please choose 'customer' or 'seller'.\n";
                continue; // Prompt again
            }

            userManager.registerUser(username, password, role); // Register the user
        }
        else {
            cout << "Invalid choice. Please try again.\n"; // Prompt if choice is invalid
        }
    }

    return 0; // End of program
}

// Login implementation in UserManager
bool UserManager::login(const string& username, const string& password, ProductManager& productManager) {
    for (User* user : users) {
        // Check for a matching username and password
        if (user->getUsername() == username && user->checkPassword(password)) {
            cout << "Login successful! Welcome, " << username << ".\n"; // Successful login message
            displayUserMenu(user, productManager); // Show appropriate user menu based on role
            return true;
        }
    }
    return false; // Indicate failed login
}

// Display user-specific menu based on role
void displayUserMenu(User* user, ProductManager& productManager) {
    while (true) {
        if (user->getRole() == "seller") { // Seller menu
            cout << "\n=== Seller Menu ===\n";
            cout << "1. Add Product\n";
            cout << "2. View All Products\n";
            cout << "3. Set Product on Sale\n";
            cout << "4. View Product Details\n";
            cout << "5. View Inventory\n"; // Added option to view inventory
            cout << "0. Logout\n";

            int choice; // Variable for seller menu choice
            cout << "Enter your choice: ";
            cin >> choice;

            Seller* seller = dynamic_cast<Seller*>(user); // Cast User to Seller

            if (!seller) {
                cout << "Invalid user type. Logging out...\n"; // Handle failure in user type casting
                break;
            }

            switch (choice) {
                case 0:
                    cout << "Logging out...\n"; // Logging out message
                    return;
                case 1:
                    seller->addProduct(productManager); // Add product
                    break;
                case 2:
                    productManager.displayAllProducts(); // Display all products
                    break;
                case 3: {
                    string productName;
                    double discount;
                    cout << "Enter product name to set on sale: ";
                    cin.ignore(); // Clear input buffer
                    getline(cin, productName);
                    cout << "Enter discount percentage: ";
                    cin >> discount;

                    // Validate discount input
                    if (discount <= 0 || discount > 100) {
                        cout << "Invalid discount percentage. Please enter a value between 1 and 100.\n";
                    } else {
                        productManager.setProductOnSale(productName, discount); // Set sale price
                    }
                    break;
                }
                case 4: {
                    string productName;
                    cout << "Enter product name to view details: ";
                    cin.ignore(); // Clear input buffer
                    getline(cin, productName);
                    productManager.displayProductDetails(productName); // Show product details
                    break;
                }
                case 5: // View Inventory option
                    productManager.displayInventory(); // Show inventory details
                    break;
                default:
                    cout << "Invalid choice. Please try again.\n"; // Prompt for valid choice
            }
        } else if (user->getRole() == "customer") { // Customer menu
            cout << "\n=== Customer Menu ===\n";
            cout << "1. View All Products\n";
            cout << "2. Search for a Product\n";
            cout << "3. View Product Details\n";
            cout << "4. Add Product to Cart\n";
            cout << "5. View Cart\n";
            cout << "6. Checkout\n";
            cout << "7. Write a Review\n";
            cout << "0. Logout\n";

            int choice; // Variable for customer menu choice
            cout << "Enter your choice: ";
            cin >> choice;

            Customer* customer = dynamic_cast<Customer*>(user); // Cast User to Customer

            if (!customer) {
                cout << "Invalid user type. Logging out...\n"; // Handle failure in user type casting
                break;
            }

            switch (choice) {
                case 0:
                    cout << "Logging out...\n"; // Logging out message
                    return;
                case 1:
                    productManager.displayAllProducts(); // Display all products
                    break;
                case 2: {
                    string query;
                    cout << "Enter search query: ";
                    cin.ignore(); // Clear input buffer
                    getline(cin, query);

                    auto& products = productManager.getProducts(); // Access all products
                    vector<Product> results; // Store search results
                    for (const auto& product : products) {
                        // Check if search query matches product name or category
                        if (product.name.find(query) != string::npos ||
                            product.category.find(query) != string::npos) {
                            results.push_back(product); // Add matching product to results
                        }
                    }

                    // Show search results
                    if (results.empty()) {
                        cout << "No products matched your search.\n"; // Notify if no results found
                    } else {
                        cout << "\nSearch Results:\n";
                        for (const auto& product : results) {
                            cout << "- " << product.name << " ($" << product.price << ") ["
                                 << product.category << "]\n"; // Display each matching product
                        }
                    }
                    break;
                }
                case 3: {
                    string productName;
                    cout << "Enter product name to view details: ";
                    cin.ignore(); // Clear input buffer
                    getline(cin, productName);
                    productManager.displayProductDetails(productName); // Show product details
                    break;
                }
                case 4: {
                    string productName;
                    int quantity;
                    cout << "Enter product name: ";
                    cin.ignore(); // Clear input buffer
                    getline(cin, productName);
                    cout << "Enter quantity: ";
                    cin >> quantity;

                    // Find the product to add to cart
                    auto& products = productManager.getProducts();
                    auto it = find_if(products.begin(), products.end(),
                        [&productName](const Product& p) { return p.name == productName; });

                    // Validate product availability and quantity
                    if (it != products.end() && it->quantity >= quantity) {
                        customer->addToCart(*it, quantity); // Add product to cart
                        productManager.updateProductQuantity(productName, quantity); // Update product quantity
                    } else {
                        cout << "Product not available in the requested quantity.\n"; // Error message for insufficient stock
                    }
                    break;
                }
                case 5:
                    customer->viewCart(); // Display the cart contents
                    break;
                case 6:
                    customer->checkout(); // Checkout
                    break;
                case 7: {
                    string productName;
                    cout << "Enter product name to review: ";
                    cin.ignore(); // Clear input buffer
                    getline(cin, productName);

                    // Check if the customer has purchased the product before allowing a review
                    if (customer->hasPurchased(productName)) {
                        string comment;
                        int rating;
                        
                        cout << "Enter rating (1-5 stars): ";
                        cin >> rating;
                        
                        // Validate rating input
                        if (rating < 1 || rating > 5) {
                            cout << "Invalid rating. Please enter a number between 1 and 5.\n"; // Error message for invalid rating
                            break;
                        }

                        cout << "Enter your review comment: ";
                        cin.ignore(); // Clear input buffer
                        getline(cin, comment);

                        productManager.addReviewToProduct(productName, user->getUsername(), comment, rating); // Add review to product
                    } else {
                        cout << "You can only review products you have purchased.\n"; // Error message for not being able to review
                    }
                    break;
                }
                default:
                    cout << "Invalid choice. Please try again.\n"; // Prompt for valid choice
            }
        } else {
            cout << "Unrecognized role. Logging out...\n"; // Log out for an unrecognized role
            break;
        }
    }
}