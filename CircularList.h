#include <iostream>
#include <bitset>
#include <stdexcept>
#include <mutex>

template<typename DataType>
struct ListNode
{
    DataType  value;
    ListNode *next;
};

template<typename DataType>
class ListAllocator
{
    typedef ListNode<DataType> Node;

    public:
        DataType* allocate(const size_t& size)
        {
            std::lock_guard<std::mutex> guard(memory_mutex);
            DataType *p = nullptr;
            size_t    i = 0;
            bool found  = false;

            while ((p == nullptr && found) || i < size)
            {
                p = allocate(i, size, found);
            }

            return p;
        }

        void deallocate()
        {
            // TODO: implement
        }

        void deallocate(const Node *p, const size_t& n)
        {
            std::lock_guard<std::mutex> guard(memory_mutex);
            size_t i = 0;

            for (; i < indexMap.size(); i++)
            {
                if (p == ((Node *)(&storage) + i))
                {
                    indexMap.reset(i);
                    p = nullptr;
                    break;
                }
            }

            size_t count = 1;

            for (size_t j = i + 1; j < indexMap.size(); ++j)
            {
                if (count < n)
                {
                    if (indexMap.test(j))
                    {
                        indexMap.reset(j);
                        auto *p = ((Node *)(&storage) + j);
                        p = nullptr;
                        ++count;
                    }
                    else
                    {
                        std::runtime_error("Error deallocating");
                    }
                }
                else
                {
                    break;
                }
            }
        }

    private:
        DataType* allocate(size_t& i, const size_t& size, bool& found)
        {
            DataType *p = nullptr;

            for (; i < indexMap.size(); i++)
            {
                if (!indexMap.test(i))
                {
                    indexMap.set(i);
                    p     = (DataType *)(&storage[i * sizeof(Node)]);
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                return nullptr;
            }

            size_t count = 1;

            for (size_t j = i + 1; j < indexMap.size(); ++j)
            {
                if (count < size)
                {
                    if (indexMap.test(j))
                    {
                        p = nullptr;
                        break;
                    }
                    else
                    {
                        indexMap.set(j);
                        ++count;
                    }
                }
                else
                {
                    break;
                }
            }

            return p;
        }

    private:
        static constexpr unsigned int MAX_DATA_SIZE = 2048;
        static constexpr size_t maxElem             = (MAX_DATA_SIZE / sizeof(Node));
        unsigned char storage[MAX_DATA_SIZE];
        static std::bitset<maxElem> indexMap;
        std::mutex memory_mutex;
};

template<typename DataType, typename Allocator = ListAllocator<DataType> >
class CircularList
{
    public:
        typedef ListNode<DataType> Node;

        // Adds new element to the end of the list
        void push_back(DataType value)
        {
            push_front(value);
            last = last->next;
        }

        // Adds new element to the beginning of the list
        void push_front(DataType value)
        {
            auto *new_node = (Node *)allocator.allocate(1);

            if (new_node == nullptr)
            {
                throw std::overflow_error("Storage full, cannot push");
            }

            new_node->value = value;
            if (last == nullptr)
            {
                last       = new_node;
                last->next = last;
            }
            else
            {
                new_node->next = last->next;
                last->next     = new_node;
            }
        }

        // Inserts new element after provided node
        void insert_after(Node *node, DataType value)
        {
            typename Allocator::template rebind<Node> nodeAllocator;
            Node *new_node = nodeAllocator.allocate(1);

            new_node->next = node->next;
            node->next     = new_node;
        }

        // Removes last element from the list and returns it
        DataType pop_back()
        {
            if (last == nullptr)
            {
                throw std::underflow_error("Nothing to pop");
            }
            else if (last == last->next)
            {
                DataType value = last->value;
                allocator.deallocate(last, 1);
                last = nullptr;
                return value;
            }

            Node *new_last = last->next;

            while (new_last->next != last)
            {
                new_last = new_last->next;
            }

            Node *head     = last->next;
            DataType value = last->value;

            allocator.deallocate(last, 1);
            new_last->next = head;
            last           = new_last;
            return value;
        }

        // Removes first element from the list end returns it
        DataType pop_front()
        {
            if (last == nullptr)
            {
                throw std::underflow_error("Nothing to pop");
            }
            else if (last == last->next)
            {
                DataType value = last->value;
                allocator.deallocate(last, 1);
                last = nullptr;
                return value;
            }

            Node *new_head = last->next->next;
            DataType value = last->next->value;

            allocator.deallocate(last->next, 1);
            last->next = new_head;
            return value;
        }

        size_t size() const
        {
            if (last == nullptr)
            {
                return 0;
            }

            Node  *p     = last->next;
            size_t count = 1;

            while (p != last)
            {
                p = p->next;
                ++count;
            }

            return count;
        }

        void print()
        {
            std::cout << "List: ";
            if (last == nullptr)
            {
                std::cout << std::endl;
                return;
            }

            Node *p = last->next;

            while (p != last)
            {
                std::cout << p->value << ", ";
                p = p->next;
            }

            std::cout << p->value << std::endl;
        }

    private:
        static Allocator allocator;
        Node *last = nullptr;
};

template<typename DataType> std::bitset<ListAllocator<DataType>::maxElem> ListAllocator<DataType>::indexMap;
template<typename DataType, typename Allocator> Allocator CircularList<DataType, Allocator>::allocator;
