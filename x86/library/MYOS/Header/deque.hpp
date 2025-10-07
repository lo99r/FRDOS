
namespace myos {

	template<typename T, int maximum_size> class Deque {

	protected:
		T arr[maximum_size];
		int start;
		int end;

	public:
		void pushBack(T value);
		void pushFront(T value);
		T popBack();
		T back();
		T popFront();
		T front();
		bool empty();
		void clear();

		int size();

		Deque<T, maximum_size>();

		T& operator[](int index) {
			index += start;
			index %= maximum_size;
			index += maximum_size;
			index %= maximum_size;
			return arr[index];
		}
	};

	template<typename T, int maximum_size>

	class Queue : public Deque<T, maximum_size> {

	public:
		void push(T value);
		T pop();

		Queue<T, maximum_size>();
	};






	template<typename T, int limit>

	T Queue<T, limit>::pop() {
		return popFront();
	}

	template<typename T, int limit>

	void Queue<T, limit>::push(T value) {
		pushBack(value);
	}

	template<typename T, int limit>

	void Deque<T, limit>::clear() {
		end = start - 1;
	}

	template<typename T, int limit>

	T Deque<T, limit>::popBack() {
		if (end < start) return 0;
		T poped = back();
		end--;
		return poped;
	}

	template<typename T, int limit>

	T Deque<T, limit>::popFront() {
		if (end < start) return 0;
		T poped = front();
		start++;
		return poped;
	}

	template<typename T, int limit>

	void Deque<T, limit>::pushBack(T value) {
		if (size() >= limit) return;
		end++;
		(*this)[size() - 1] = value;
	}

	template<typename T, int limit>

	void Deque<T, limit>::pushFront(T value) {
		if (size() >= limit) return;
		start--;
		(*this)[0] = value;
	}

	template<typename T, int limit>

	bool Deque<T, limit>::empty() {
		return size() == 0;
	}

	template<typename T, int limit>

	T Deque<T, limit>::front() {
		return (*this)[0];
	}

	template<typename T, int limit>

	T Deque<T, limit>::back() {
		return (*this)[end];
	}

	template<typename T, int limit>

	int Deque<T, limit>::size() {
		return end - start + 1;
	}

	template<typename T, int limit>

	Deque<T, limit>::Deque() {
		start = 0;
		end = -1;
	}

	template<typename T, int max_size>

	Queue<T, max_size>::Queue() : Deque<T, max_size>() {

	}
}