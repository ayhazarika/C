#include <cassert>
#include <iomanip>
#include <iostream>

#include "project4.hpp"
#include "History.hpp"
#include "Transaction.hpp"

////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////
//
//

// Constructor
Transaction::Transaction( std::string ticker_symbol, unsigned int day_date, unsigned int month_date,  unsigned int year_date, bool buy_sell_trans,  unsigned int number_shares, double trans_amount ){
  //initializes PRIVATE member variables 
  symbol = ticker_symbol;
  day = day_date;
  month = month_date;
  year = year_date;
  
  shares = number_shares;
  amount = trans_amount;
  trans_id = assigned_trans_id++;

  if(buy_sell_trans){trans_type = "Buy";}
  else{trans_type = "Sell";}
 
  acb = 0;
  acb_per_share = 0;
  share_balance = 0;
  cgl = 0;

  p_next = nullptr;

}


// Destructor
Transaction::~Transaction(){}
  

// Overloaded < operator - determines which
//OVERLOAD the "less than" operator - i.e. create a method of comparing transaction objects by their date
 bool Transaction::operator<( Transaction const &other ){
  if(year != other.get_year()){
    return (year < other.get_year());
  }
  else if(month != other.get_month()){
    return (month < other.get_month());
  }
  else if(day != other.get_day()){
    return (day < other.get_day());
  }
  else{
    return (trans_id < other.get_trans_id());
  }
}   


// GIVEN
// Member functions to get values.
//
std::string Transaction::get_symbol() const { return symbol; }
unsigned int Transaction::get_day() const { return day; }
unsigned int Transaction::get_month() const { return month; }
unsigned int Transaction::get_year() const { return year; }
unsigned int Transaction::get_shares() const { return shares; }
double Transaction::get_amount() const { return amount; }
double Transaction::get_acb() const { return acb; }
double Transaction::get_acb_per_share() const { return acb_per_share; }
unsigned int Transaction::get_share_balance() const { return share_balance; }
double Transaction::get_cgl() const { return cgl; }
bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true: false ; }
unsigned int Transaction::get_trans_id() const { return trans_id; }
Transaction *Transaction::get_next() { return p_next; }
 


// GIVEN
// Member functions to set values.
//
void Transaction::set_acb( double acb_value ) { acb = acb_value; }
void Transaction::set_acb_per_share( double acb_share_value ) { acb_per_share = acb_share_value; }
void Transaction::set_share_balance( unsigned int bal ) { share_balance = bal ; }
void Transaction::set_cgl( double value ) { cgl = value; }
void Transaction::set_next( Transaction *p_new_next ) { p_next = p_new_next; }


// GIVEN
// Print the transaction.
//
void Transaction::print() {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << std::setw(4) << get_trans_id() << " "
    << std::setw(4) << get_symbol() << " "
    << std::setw(4) << get_day() << " "
    << std::setw(4) << get_month() << " "
    << std::setw(4) << get_year() << " ";


  if ( get_trans_type() ) {
    std::cout << "  Buy  ";
  } else { std::cout << "  Sell "; }

  std::cout << std::setw(4) << get_shares() << " "
    << std::setw(10) << get_amount() << " "
    << std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
    << std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
    << std::setw(10) << std::setprecision(3) << get_cgl()
    << std::endl;
  }

 

////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////

// Constructor
History::History(){
  p_head = nullptr;
}

// Destructor
History::~History(){
  Transaction *p_current = p_head;

  //Traverses through list and deletes each node
  while(p_current != nullptr){
    Transaction *p_temporary = p_current->get_next();
    delete p_current;
    p_current = p_temporary;
  }

  p_head = nullptr;
}

// insert(...): Insert transaction into linked list.
void History::insert(Transaction *p_new_trans){
  //checks if linked list is emptyae
  if(p_head == nullptr){
    p_head = p_new_trans;
    p_head->set_next(nullptr);
  }
  else{
    //traverse the linked list to find last node
    Transaction *p_last_node = p_head;
    while(p_last_node->get_next() != nullptr){
      p_last_node = p_last_node->get_next();
    }

    //change pointers such that last node points to the new node, and the new node points to nullptr (effectively appends the node to the end of the list)
    p_last_node->set_next(p_new_trans);
    p_new_trans->set_next(nullptr);
  }
  
}

// read_history(...): Read the transaction history from file.
void History::read_history(){
  //opens the reading file
  ece150::open_file();

  //while a next transaction entry exists, continues reading in info
  while(ece150::next_trans_entry()){
    //creates new_node object and inserts it to the back of the linked list using the insert function
    Transaction *p_new_node = new Transaction(ece150::get_trans_symbol(), ece150::get_trans_day(), ece150::get_trans_month(), ece150::get_trans_year(), ece150::get_trans_type(), ece150::get_trans_shares(), ece150::get_trans_amount());
    insert(p_new_node);
  }

  //closes the reading file
  ece150::close_file();
}

// print() Print the transaction history.
void History::print(){
  //prints out first line
  std::cout << "========== BEGIN TRANSACTION HISTORY ============" << std::endl;

  //checks if linked list is empty first
  if(p_head != nullptr){
    Transaction *p_current_node_ = p_head;

    //print every node up until the last node is found
    while(p_current_node_->get_next() != nullptr){
      p_current_node_->print();
      p_current_node_ = p_current_node_->get_next();
    }

    //print the last node
    p_current_node_->print();
  }

  //prints out last line
  std::cout << "========== END TRANSACTION HISTORY ============" << std::endl;
}

// sort_by_date(): Sort the linked list by trade date.
//IMPLEMENTING SELECTION SORT w/ "TWO LISTS"
void History::sort_by_date(){
  //initialize sorted pointer 
  Transaction *sorted_tail_ = nullptr;    //points to the last pointer in the sorted list
  Transaction *sorted_head_ = nullptr;    //points to the head of the sorted list

  //while loop - continues until existing linked list isn't of 1 element (i.e. head doesn't point to nullptr)
  while(p_head != nullptr){
    //intiailize pointers
    Transaction *current_node_ = p_head;
    Transaction *current_prior = nullptr;
    Transaction *oldest_transaction_ = p_head;
    Transaction *oldest_prior_ = nullptr;

    //traverses the linked list and finds the largest value in the list
    while(current_node_ != nullptr){
      if(current_node_->operator<(*oldest_transaction_)){
        //save the pointer to largest value
        oldest_prior_ = current_prior;
        oldest_transaction_ = current_node_;
      }
      //increments current node and current prior
      current_prior = current_node_;
      current_node_ = current_node_->get_next();
    }

    //"remove" oldest transaction from linked list (i.e. change pointer of node prior to it)
    if(oldest_prior_ != nullptr){
      oldest_prior_->set_next(oldest_transaction_->get_next());
    }
    else{
      p_head = p_head->get_next();
    }    

    //add the largest value pointer to the "sorted" linked list (make it the last element in the list)
    if(sorted_head_ == nullptr){
      sorted_head_ = oldest_transaction_;
      sorted_tail_ = oldest_transaction_;
    }
    else{
      sorted_tail_->set_next(oldest_transaction_);
      sorted_tail_ = sorted_tail_->get_next();
    }

    //sets the new last element's next pointer to nullptr
    sorted_tail_->set_next(nullptr);
  }
    
  //after there is only one element left (which is the smallest element), add it to the "sorted" linked list
  //(ACCOUNTS for if the list is empty)
  if(sorted_head_ != nullptr){
    sorted_tail_->set_next(p_head);
  }
  
  //change head pointer to now point where sorted pointer currently is
  p_head = sorted_head_;
} 

// update_acb_cgl(): Updates the ACB and CGL values.
void History::update_acb_cgl(){
  //pointers to walk the linked list
  Transaction *search = p_head;
  Transaction *previous = p_head;
  double acb = 0.0;
  double share_balance = 0.0;

  //traverses linked list
  while(search != nullptr){
    if(search->get_trans_type()){
      //calculates and updates ACB
      acb += search->get_amount();
      search->set_acb(acb);

      //calculates and updates Share Balance
      share_balance += search->get_shares();
      search->set_share_balance(share_balance);

      //calculates and updates ACB/Share
      search->set_acb_per_share(acb/share_balance);
    }
    else{
       //calculates and updates ACB
      acb -= (search->get_shares()*previous->get_acb_per_share());
      search->set_acb(acb);

      //calculates and updates Share Balance
      share_balance -= search->get_shares();
      search->set_share_balance(share_balance);

      //calculates and updates ACB/Share
      search->set_acb_per_share(acb/share_balance);

      //calculates cgl for each node
      search->set_cgl(search->get_amount() - ((search->get_shares())*(previous->get_acb_per_share())));
    }

  previous = search;
  search = search->get_next();

  }
}

// compute_cgl(): )Compute the ACB, and CGL
//Only for SELL Transactions
double History::compute_cgl(unsigned int year){
  //find desired transaction by walking the linked list
  Transaction *search = p_head;
  Transaction *previous = p_head;
  double total_year_cgl{0.0};

  while(search != nullptr){
     double current_cgl{0.0};

    //calculate and set CGL, and also checks if 
    if(!search->get_trans_type()){
      current_cgl = search->get_amount() - ((search->get_shares())*(previous->get_acb_per_share()));
      search->set_cgl(current_cgl);
      
      if(search->get_year() == year){
        total_year_cgl += current_cgl;
      }
    }
    
    //incrementing
    previous = search;
    search = search->get_next();
  }

  //return the total CGL for the specified year
  return (total_year_cgl);
}

// GIVEN
// get_p_head(): Full access to the linked list.
Transaction *History::get_p_head() { return p_head; }