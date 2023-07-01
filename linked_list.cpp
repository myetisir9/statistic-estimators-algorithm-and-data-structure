/*
Mustafa Yetişir
150170032

ASSIGNMENT2

02.12.2022

Linked List File: Contains Linked List and Linked List Node classes and their functions to use them when calculating estimators and so on.
*/



class LinkedListNode	//node class for the linked list structure, which has value and next ptr and appropriate getters and setters
{
	double _value;
	LinkedListNode* _next = nullptr;

public:
	double get_value() {return _value;}	//getter for linked list node
	LinkedListNode* get_next() { return _next;}	//getter for next node
	void set_next(LinkedListNode* next) { _next = next;}	//setter for next node

	LinkedListNode(double value, LinkedListNode* next) : _value(value), _next(next) {};
	~LinkedListNode() {if (_next) delete _next;};
};


class LinkedList//My Linked List Class
{
private:
	LinkedListNode* head = nullptr;
	
public:
	void insert(double);		//the insert function of my linked list
	LinkedListNode* get_head() {return head;};	//getter for the head node of my linked list
	double get_kth(int k);	//function to get kth value of the linked list
	double get_average_of_two_at(int k);	
	~LinkedList();
};

void LinkedList::insert(double value)//the insert function of my linked list. Thanks to this, I will have a sorted in descending order. Head will have the max and tail will have the min.
{
	if (head == nullptr)
	{
		head = new LinkedListNode(value, nullptr);
		return;
	}

	auto it = head;
	while (it->get_next() != nullptr && value < it->get_next()->get_value())
		it = it->get_next();
	
	LinkedListNode* new_node = new LinkedListNode(value, it->get_next());
	it->set_next(new_node);
}

double LinkedList::get_kth(int k)//returns kth element of the list
{
	auto it = head;
	while (k--)
		it = it->get_next();
	return it->get_value();
}

double LinkedList::get_average_of_two_at(int k)//avg of 2 elements which were indexed in k and k+1
{
	return (get_kth(k) + get_kth(k+1)) / 2.0;
}


LinkedList::~LinkedList()//linked list destructor, we delete head and it's done.
{
	if (head != nullptr)
		delete head;
}
