/**
 * @description Javascript like arrays
 * @author Jaenster
 * @license MIT
 */

#ifndef JAVASCRIPT_LIKE_ARRAY_H
#define JAVASCRIPT_LIKE_ARRAY_H

#include <cstdint>
#include <functional>

template<typename T>
class Collection {
private:
    typedef Collection<T> self;
protected:
    struct Node {
        Node() {
            cur = nullptr;
            prev = nullptr;
            next = nullptr;
        }

        T *cur;
        Node *prev = nullptr;
        Node *next = nullptr;
    };

    Node *last = nullptr;
    Node *first = nullptr;
    Node *lastAccessed = nullptr;

    template<typename U>
    void loopTroughEach(U (cb)(T *), std::function<U(T &, U)> handler) {
        Node *tmp = this->first;
        T *cur;
        do {
            if ((cur = tmp->cur) != nullptr && !handler(cur, cb(cur))) break;
        } while ((tmp = tmp->next) != nullptr);
    }

    int length = 0;

private:
    self sliceSplice(int from, int items = INT32_MAX, bool del = false) {
        Collection<T> newCol = *new Collection<T>;

        if (from < 0) from = length + from; // +- stands for - here
        if (items == INT32_MAX) items = length - from; // everything left from this position

        int i = 0;
        for (Node *node = this->first; node != nullptr; node = node->next, i++) {
            if (i >= from && i < from + items) {
                newCol.push(node->cur);
                if (del) {// remove this current element from the list
                    // If this element is both the first & the last.
                    if (node == this->first && node == this->last) {
                        this->first = this->last = nullptr;
                    } else if (node == this->first) {
                        // its the first node, so there isnt a previous one
                        this->first = node->next; // if nullptr that is fine too
                        node->next->prev = nullptr; // the next node will be the first one, it doesnt have a previous one
                    } else if (node == this->last) {
                        this->last = node->prev; // if nullptr that is fine too
                        // the previous node is the last one, doesnt have a parent
                        node->prev->next = nullptr;
                    } else {
                        auto next = node->next;
                        auto prev = node->prev;
                        next->prev = node->prev;
                        prev->next = node->next;

                    }
                }
            }
        }
        return newCol;
    }

public:
    /////////////////////////////////////
    // Add/remove functions
    ////////////////////////////////////
    T &pop() {
        if (!this->last) return nullptr;

        T &ret = this->last;
        if (this->last == this->first) {
            this->first = this->last = nullptr;
        } else {
            this->last->prev->next = nullptr;
            this->last = this->last->prev;
        }
        return ret;
    }

    self *push(T *newInstance) {
        Node *newNode = new Node();

        newNode->cur = newInstance;
        // if there is a last node, this is the next obj of the previous last
        if (last != nullptr) {
            last->next = newNode;
            newNode->prev = last;
        }
        last = newNode;
        // this is the first
        if (first == nullptr) first = newNode;
        length++;
        return this;
    }

    self *push(T newInstance) {

        return push(&newInstance);
    }

    // same as pop but with the last element
    T &shift() {
            if (this->first == nullptr) return nullptr;

        T &ret = this->first;
        if (this->first->next != nullptr) {
            this->first->next->prev = nullptr; // its the first
            this->first = this->first->next;
        }
    }

    ////////////////////////////////////
    // Loop overs (forEach/filter/reduce)
    ///////////////////////////////////
    typedef void (*voidCB)(T &);

//    void forEach(voidCB cb) {
    void forEach(std::function<void(T &current)> cb) {
        Node *tmp = this->first;
        bool lastOne;
        for (Node *node = this->first; node != nullptr; node = node->next) cb(*node->cur);
    }

    self filter(bool (*cb)(T &)) {
        Collection<T> newCol = *new Collection<T>;

        for (Node *node = this->first; node != nullptr; node = node->next) {
            if (cb(*node->cur)) newCol.push(node->cur);
        }

        return newCol;
    }

    template<class U>
    U reduce(std::function<U(U accumulator, T &current)> cb, U initial) {
        U val = initial;

        for (Node *node = this->first; node != nullptr; node = node->next)
            val = cb(val, *node->cur);

        return val;
    }

    template<class U>
    Collection<U> map(std::function<U(T &current)> cb) {
        Collection<U> newCol = *new Collection<U>;
        for (Node *node = this->first; node != nullptr; node = node->next)

            newCol.push(cb(*node->cur));

        return newCol;
    }


    ////////////////////////////////////
    // modifiers (slice/splice/concat)
    ///////////////////////////////////

    Collection<T> slice(int from, int items = INT32_MAX) {
        return sliceSplice(from, items, false);
    }

    Collection<T> splice(int from, int items = INT32_MAX) {
        return sliceSplice(from, items, true);
    }

    Collection<T> concat(self *_1, self *_2, self *_3, self *_4, self *_5, self *_6, self *_7, self *_8, self *_9) {
        Collection<T> newCol = *new Collection<T>;
        self *test[4] = {this, _1, _2, _3};

        auto adder = [&](T &val) {
            newCol.push(&val);
        };

        for (int i = 0; i < 3; i++) {
            if (test[i]) test[i]->forEach(adder);
        }
        return newCol;
    }

    // concat's alternative function signatures
    self concat(self &_1, self &_2, self &_3, self &_4, self &_5, self &_6, self &_7, self &_8) {
        return concat(&_1, &_2, &_3, &_4, &_5, &_6, &_7, &_8, NULL);
    }

    self concat(self &_1, self &_2, self &_3, self &_4, self &_5, self &_6, self &_7) {
        return concat(&_1, &_2, &_3, &_4, &_5, &_6, &_7, NULL, NULL);
    }

    self concat(self &_1, self &_2, self &_3, self &_4, self &_5, self &_6) {
        return concat(&_1, &_2, &_3, &_4, &_5, &_6, NULL, NULL, NULL);
    }

    self concat(self &_1, self &_2, self &_3, self &_4, self &_5) {
        return concat(&_1, &_2, &_3, &_4, &_5, NULL, NULL, NULL, NULL);
    }

    self concat(self &_1, self &_2, self &_3, self &_4) {
        return concat(&_1, &_2, &_3, &_4, NULL, NULL, NULL, NULL, NULL);
    }

    self concat(self &_1, self &_2, self &_3) { return concat(&_1, &_2, &_3, NULL, NULL, NULL, NULL, NULL, NULL); }

    self concat(self &_1, self &_2) { return concat(&_1, &_2, NULL, NULL, NULL, NULL, NULL, NULL, NULL); }

    self concat(self &_1) {
        return concat(&_1, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    }

    /////////////////////////////////////
    // testers (every/some)
    ////////////////////////////////////
    bool every(std::function<bool(T &)> cb) {
        bool every = true;
        for (Node *node = this->first; node != nullptr && every; node = node->next) every = every && cb(*node->cur);
        return every;
    }


    Collection() = default;

};

#endif //JAVASCRIPT_LIKE_ARRAY_H