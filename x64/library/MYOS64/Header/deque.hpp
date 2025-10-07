template<typename T>

class deque {
protected:
	T* arr;
	int start;
	int end;
	int limit;

public:
	void push_back(T value);
	void push_front(T value);
	T pop_back();
	T back();
	T pop_front();
	T front();
	bool empty();
	void clear();

	int size();

	deque<T>(T* pointer, int limit_size);

	T& operator[](int index) {
		index += start;
		index %= limit;
		index += limit;
		index %= limit;
		return arr[index];
	}
};

template<typename T>

class queue : deque<T> {
private:
	using deque<T>::push_back;
	using deque<T>::back;
	using deque<T>::pop_front;
	using deque<T>::arr;
	using deque<T>::limit;
	using deque<T>::start;
	using deque<T>::end;
public:
	using deque<T>::clear;
	using deque<T>::empty;
	using deque<T>::size;
	using deque<T>::front;
	void push(T value);
	T pop();

	queue<T>(T* pointer, int limit_size);
};






template<typename T>

T queue<T>::pop() {
	return pop_front();
}

template<typename T>

void queue<T>::push(T value) {
	push_back(value);
}

template<typename T>

void deque<T>::clear() {
	end = start - 1;
}

template<typename T>

T deque<T>::pop_back() {
	if (end < start) return 0;
	T poped = back();
	end--;
	return poped;
}

template<typename T>

T deque<T>::pop_front() {
	if (end < start) return 0;
	T poped = front();
	start++;
	return poped;
}

template<typename T>

void deque<T>::push_back(T value) {
	if (size() > limit) return;
	end++;
	(*this)[size() - 1] = value;
}

template<typename T>

void deque<T>::push_front(T value) {
	if (size() > limit) return;
	start--;
	(*this)[0] = value;
}

template<typename T>

bool deque<T>::empty() {
	return size() == 0;
}

template<typename T>

T deque<T>::front() {
	return (*this)[0];
}

template<typename T>

T deque<T>::back() {
	return (*this)[end];
}

template<typename T>

int deque<T>::size() {
	return end - start + 1;
}

template<typename T>

deque<T>::deque(T* pointer, int limit_size) {
	arr = pointer;
	limit = limit_size;
	start = 0;
	end = -1;
}

template<typename T>

queue<T>::queue(T* pointer, int limit_size) : deque<T>(pointer, limit_size) {

}