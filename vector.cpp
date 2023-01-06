#include<iostream>
#ifndef _MY_VECTOR_CPP
#define _MY_VECTOR_CPP

namespace mystd {
    template <typename T>
    class vector {
        #define _DEFUALT_SIZE 10
        public:
            typedef std::size_t size_type;
            typedef T           value_type;
            typedef T&          reference;
            typedef T*          iterator;
        private:
            iterator start;
            iterator finish;
            iterator end_of_storage;

            void _create(size_type size = _DEFUALT_SIZE) {
                if(size <= 0) {
                    size = _DEFUALT_SIZE;
                }
                start = new T[size]();
                finish = start;
                end_of_storage = start + size;
            } 

            void _create(size_type size, const T& x) {
                start = new T[size]();
                for(size_type i = 0;i < size;++i) {
                    start[i] = x;
                }
                end_of_storage = start + size;
                finish = end_of_storage;
            }

            void _inside_copy(iterator first, iterator end, iterator new_first) {
                if(new_first == first) {
                    return;
                }
                if(new_first < first || new_first >= end_of_storage) {
                    for(;first != end;++new_first, ++first) {
                        *new_first = *first;
                    }
                }
                else { //* first < new_first < end_of_storage
                    iterator temp1 = new_first - first + end - 1;
                    iterator temp2 = end - 1;
                    for(;temp2 >= first;--temp1, --temp2) {
                        *temp1 = *temp2;
                    }
                }
            }

            void _exterior_copy(iterator first, iterator end, iterator new_first) {
                for(;first != end;++first, ++new_first) {
                    *new_first = *first;
                }
            }

            void _realloc(size_type size) {
                if(size < finish - start) {
                    finish = start;
                    return;
                }
                else {
                    delete[] start;
                    start = new T[size]();
                    finish = start;
                    end_of_storage = start + size;
                }
            }

            void _keep_realloc(size_type size) {
                iterator temp = new T[size]();
                _exterior_copy(start, finish, temp);//*值传递
                finish = temp + (finish - start);
                delete[] start;
                start =  temp;
                end_of_storage = start + size;
            }

        public:
            vector() {
                _create();
            }

            vector(size_type size) {
                _create(size);
            }

            vector(size_type size, const T& x) {
                _create(size, x);
            }

            vector(vector<T>& v) {
                _create(v.capacity());
                _exterior_copy(v.begin(), v.end(), start);
                finish = start + v.size();
            }

            vector(iterator first, iterator end) {
                if(first != nullptr && end != nullptr) {
                    size_type size = end - first;
                    _create(size * 2);
                    _exterior_copy(first, end, start);
                    finish = start + size;
                }
                else {
                    _create();
                }
            }

            template <typename ...Args>
            vector(const Args& ...args) {
                size_type size = sizeof...(args);
                _create(size * 2);
                T temp[] = {args...};
                _exterior_copy(temp, temp+size, start);
                finish = start + size;
            }


            size_type size() { return finish - start; }

            size_type capacity() { return end_of_storage - start; }

            iterator begin() { return start; }

            iterator end()   { return finish; }

            reference front() { return *start; }

            reference back() { return *(finish - 1); } 
            
            void operator=(vector<T>&& v) {
                swap(v);
            } 

            void operator=(vector<T>& v) {
                if(capacity() < v.capacity()) {
                    _realloc(v.capacity());
                }
                else {
                    clear();
                }
                _exterior_copy(v.begin(), v.end(), start);
            }

            bool empty() {return start == finish; }

            bool operator==(vector<T>& v) { return start == v.start; }

            void push_back(const T& x) {
                if(finish != end_of_storage) {
                    *finish++ = x;
                }
                else {
                    _keep_realloc(size() * 2);
                    *finish++ = x;
                }
            }

            void pop_back() {
                if(finish != start) {
                    finish--;
                }
            }

            reference operator[] (size_type i) {
                return start[i];
            }

            iterator erase(iterator first, iterator end) {
                if(first >= start && end <= end_of_storage) {
                    _inside_copy(end, finish, first);
                    finish -= end - first;
                }
                return first;
            }

            iterator erase(iterator first) {
                return erase(first, first + 1);
            }

            iterator clear() {
                return erase(start, finish);
            }

            iterator insert(iterator first, size_type size, const T& x) {
                if(first >= start && first < end_of_storage) {
                    if(finish + size >= end_of_storage) {
                         size_type temp = finish - first;
                         _keep_realloc((this->size() + size) * 2);
                         first = finish - temp;//* realloc之后就换地址了
                    }
                    _inside_copy(first, finish, first + size);
                    for(size_type i = 0;i < size;++i) {
                        first[i] = x;
                    }
                    finish += size;
                }
                return first;
            } 

            iterator insert(iterator first, const T& x) {
                return insert(first, 1, x);
            }

            iterator insert(iterator _first, iterator array_first, iterator array_end) {
                if(_first >= start && _first < end_of_storage) {
                    size_type size = array_end - array_first;
                    if(finish + size >= end_of_storage) {
                         size_type temp = finish - _first;
                         _keep_realloc((this->size() + size) * 2);
                         _first = finish - temp;//* realloc之后就换地址了
                    }
                    _inside_copy(_first, finish, _first + size);
                    _exterior_copy(array_first, array_end, _first);
                    finish += size;
                }
                return _first;
            }

            void resize(size_type size) {
                if(size <= this->size()) {
                    finish = start + size;
                }
                else {
                    _keep_realloc(size * 2);
                    finish = start + size;                    
                }
            }

            void resize(size_type size, const T& x) {
                if(size <= this->size()) {
                    finish = start + size;
                }
                else {
                    insert(finish, size - this->size(), x);
                    finish = start + size;
                }
            }

            void reserver(size_type size) {
                if(size > this->size()) {
                    _keep_realloc(size);
                }
            }

            void swap(vector<T>& v) {
                iterator temp = v.start;
                v.start = start;
                start = temp;
                
                temp = v.finish;
                v.finish = finish;
                finish = temp;

                temp = v.end_of_storage;
                v.end_of_storage = end_of_storage;
                end_of_storage = temp;
            }

            void swap(vector<T>&& v) {
                iterator temp = v.start;
                v.start = start;
                start = temp;
                
                temp = v.finish;
                v.finish = finish;
                finish = temp;

                temp = v.end_of_storage;
                v.end_of_storage = end_of_storage;
                end_of_storage = temp;
            }

            iterator find(const T& x) {
                iterator temp;
                for(temp = start;temp != finish;++temp) {
                    if(*temp == x) {
                        break;
                    }
                }
                return temp; 
            }

            reference at(size_type i) {
                if(i >= size()) {
                    throw "Array out of bounds";
                }
                    return start[i];
            }

            void realloc(size_type size) {
                _realloc(size);
            }

            void _add_finish(size_type i) {
                finish += i;
            }
    };
}
#endif