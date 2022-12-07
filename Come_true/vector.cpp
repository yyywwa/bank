#define defaultSize 10 
#define multiplication_factor 2
#include<iostream>
template <typename P>
static int arraySize(P&array){return sizeof(array)/sizeof(array[0]);}
template <typename T>
class vector {
public:
	typedef T		    value_type;
	typedef value_type* pointer;
	typedef value_type* iterator;
	typedef value_type& reference;
	typedef size_t		size_type;
	typedef ptrdiff_t	difference_type;
private:
	iterator start = nullptr;//当前使用的头
	iterator finish;//当前使用的尾
	iterator end_of_storage;//可用空间的尾

	iterator copy(iterator first, iterator last, iterator new_first) {//用于同一数组间
		if (new_first == first)
			return new_first;
		if (new_first < first) {
			iterator temp1 = new_first;
			iterator temp2 = first;
			for (; temp2 < last; ++temp1, ++temp2)
				*(temp1) = *(temp2);
		}
		else {
			iterator temp1 = new_first + (last - first) - 1;
			iterator temp2 = last - 1;
			for (; temp2 >= first; --temp1, --temp2){
				*(temp1) = *(temp2);
			}
		}
		return new_first;
	}//左闭右开

	void _copy(iterator first, iterator last, iterator new_first) {//用于不同数组间
		for (; first != last; ++first, ++new_first)
			*new_first = *first;
	}//左闭右开

	void fill_initialize(size_type n, const T& value) {
		start = new T[n]();
		for (size_type i = 0; i < n; ++i) {
			start[i] = value;
		}
		finish = start + n;
		end_of_storage = finish;
	}

	void _keep_realloc(size_type new_size) {//保留相对位置
		if (new_size > end_of_storage - start) {
			size_type location_size = finish - start;
			iterator temp = start;
			start = new T[new_size]();
			_copy(temp,temp+location_size,start);
			delete[] temp;
			finish = start + location_size;
			end_of_storage = start + new_size; 
		}
	}

	void _redouble(size_type factor = multiplication_factor) {
		_keep_realloc(factor * (end_of_storage - start));}//扩容

	void _realloc(size_type size) {//清空原容器，创建新容器
		delete[] start;
		start = new T[size]();
		finish = start;
		end_of_storage = start + size;
	}

	void _creat(size_type n = defaultSize) {//默认构造动态数组
		start = new T[n]();
		finish = start;
		end_of_storage = start + n;
		//std::cout<<"what ther is vector creat"<<std::endl;
	}

public:
	vector() { _creat(); }

	vector(size_type n, const T& value) { fill_initialize(n, value); }

	vector(int n, const T& value) { fill_initialize(n, value); }

	vector(long n, const T& value) { fill_initialize(n, value); }

	explicit vector(size_type n) { _creat(n); }

	template<typename ...Args>
	vector(const Args& ...args) {//使其可以使用{}进行初始化，如vector<int> a = {1,2,3,4,5} or a(1,2,3,4,5);
		size_type size = sizeof...(args);
		if (size > defaultSize)
			_creat();
		else
			_creat(size * 2);
		T temp[] = { args... };
		for (size_type i = 0; i < size; ++i)
			push_back(temp[i]);
	}

	template <typename P>
	vector(P& first) {//用于引用指针，用iterator&的会报错，用iterator会让size一直为2
		size_type size = arraySize(first);
		if (defaultSize >= size)
			_creat();
		else
			_creat(size * 2);
		_copy(first, first + size, start);
		finish = start + size;
	}

	vector(vector& V) {
		_creat(V.capacity());
		_copy(V.begin(), V.end(), start);
		finish = start + V.size();
	}

	void operator=(vector& V) {
		if (this != &V) {
			if (capacity() < V.size())
				_realloc(V.capacity());
			_copy(V.begin(), V.end(), start);
			finish = start + V.size();
		}
	}

	template <typename P>
	void operator=(P& first) {//用于引用指针，用iterator&的会报错，用iterator会让size一直为2
		size_type size = arraySize(first);
		if (capacity() < size)
			_realloc(size * 2);
		_copy(first, first + size, start);
		finish = start + size;
	}

	vector(iterator first, iterator last) {//左闭右开 传入数组时需将最大值加一，传入vector则不需要
		size_type size = size_type(last - first);
		if (defaultSize >= size)
			_creat();
		else
			_creat(size * 2);
		_copy(first, last, start);
		finish += size;
	}

	~vector() {
		//std::cout<<"delete vector size = "<<end_of_storage - start<<std::endl;
		delete[] start;}

	void push_back(const T& x) {
		if (finish != end_of_storage) {
			*(finish) = x;
			++finish;
		}
		else {
			_redouble();
			*(finish) = x;
			++finish;
		}
	}

	void pop_back() { if (start != finish)--finish; }

	iterator _erase(iterator position) {
		if (position + 1 != finish)
			copy(position + 1, finish, position);
		--finish;
		return position;
	}

	iterator _erase(iterator first, iterator last) {
		copy(last, finish, first);
		finish = finish - (last - first);
		return first;
	}

	iterator _insert(iterator first, size_type n, const T& value) {
		if(first == 0)
			first = start;
		if (finish + n > end_of_storage){
			size_type location_size = first - start;
			_keep_realloc((size() + n) * multiplication_factor);
			first = start + location_size;
		}
		copy(first, finish, first + n);
		iterator temp = first;
		for (size_type i = 0; i < n; ++i)
			*(temp++) = value;
		finish += n;
		return temp;
	}

	iterator _insert(iterator first, size_type n, iterator _first) {
		if (finish + n > end_of_storage){
			size_type location_size = first - start;
			_keep_realloc((size() + n) * multiplication_factor);
			first = start + location_size;
		}
		copy(first, finish, first + n);
		for (size_type i = 0; i < n; ++i)
			*(first + i) = *(_first + i);
		finish += n;
		return first;
	}

	void show() {
		size_type size = this->size();
		for (size_type i = 0; i < size; ++i) {
			std::cout << *(start + i) << " ";
		}
		std::cout << std::endl;
	}
//给外部使用的函数
	void print(size_type i) { std::cout << *(start + i); }

	void keep_realloc(size_type size) { _keep_realloc(size); }

	void realloc(size_type size) { _realloc(size); }//用于在类外重置vector

	void redouble(size_type factor = multiplication_factor ) {_redouble(factor);}

	iterator insert(iterator first, const T& value) { return _insert(first, 1, value);}

	iterator insert(size_type first, const T& value) { return _insert(start + first, value); }

	iterator insert(size_type first, size_type n, const T& value) { return _insert(start + first, n, value); }

	iterator insert(size_type first, size_type n, const iterator _first) { return _insert(start + first, n, _first); }

	iterator erase(size_type i) { return _erase(start + i);}

	iterator erase(size_type i, size_type n) { return _erase(start + i, start + n);}

	void setLocationSize(size_type new_size) { finish = start + new_size; }

	void add_finish(size_type n = 1){finish += n;};

	void clear() { finish = start; }/*erase(begin(),end())*/

	reference operator[](size_type n) {
		if(n >= capacity())
			_keep_realloc(n * 2);
		if(n >= size())
			finish = start + n + 1;
		return *(start+n);
	}

	reference at(size_type n) {
		if (n < size())
			return *(start + n);
		else
			throw "Crossing the line";
	}

	reference front() { return *start; }

	reference back() { return *(finish - 1); }

	iterator begin() { return start; }

	iterator end() { return finish; }

	size_type size() { return size_type(finish - start); }

	size_type capacity() { return size_type(end_of_storage - start); }

	bool empty() const { return finish == start; }

	iterator find(iterator first, iterator last, const T& value) {
		for (; first != last; ++first)
			if (*first == value)
				break;
		return first;
	}

};
