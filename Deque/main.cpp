#include <iostream>

#define BLOCK_SIZE 8

template<class T>
class deque {
private:

	T** deq;
	T* last_item;
	T* first_item;
	T* first_block;
	T* last_block;
	int size;
	int block_count;
	int which_first_block;
	int which_last_block;
	T* at(int i)
	{
		if (i >= size)
		{
			std::cout << "Index out of bounds!\n";
			return (first_item);
		}

		int which_block = i / BLOCK_SIZE + which_first_block;
		if (which_block == 0)
		{
			if (first_item + i >= first_block + BLOCK_SIZE)
			{
				int how_many_elem_in_last_block = i - (first_item + i - first_block) % (BLOCK_SIZE);
				return &(deq[1 + which_first_block][i - how_many_elem_in_last_block]);
			}
			return (first_item + i);
		}
		if (i % BLOCK_SIZE + (first_item - first_block) + deq[which_block] >= deq[which_block] + BLOCK_SIZE)
		{
			return &(deq[which_block + 1][(i % BLOCK_SIZE + (first_item - first_block)) % BLOCK_SIZE]);
		}
		return &(deq[which_block][i % BLOCK_SIZE + (first_item - first_block)]);
	}


public:
	deque() {
		deq = new T * [1], deq[0] = new T[BLOCK_SIZE];
		last_block = first_block = deq[0];
		last_item = first_item = &deq[0][BLOCK_SIZE / 2];
		size = 0, block_count = 1;
		which_first_block = which_last_block = 0;
	}
	void push_back(T key)
	{
		if (size == 0)
		{
			last_block[BLOCK_SIZE / 2] = key;
			++size;
			return;
		}
		else if (last_item + 1 >= last_block + BLOCK_SIZE)
		{
			T** aux = new T * [block_count + 1];
			int cnt = 0;
			for (int i = which_first_block; i < block_count + which_first_block; ++i)
			{
				aux[cnt] = deq[i];
				++cnt;
			}
			aux[block_count] = new T[BLOCK_SIZE];
			delete[] deq;
			deq = aux;

			++size;
			++block_count;
			last_block = deq[block_count - 1];
			last_block[0] = key;
			last_item = &(last_block[0]);
			which_first_block = 0;
			which_last_block = block_count - 1;
			return;
		}
		*(last_item + 1) = key;
		++size;
		last_item = last_item + 1;
	}

	void push_front(T key)
	{
		if (size == 0)
		{
			first_block[BLOCK_SIZE / 2] = key;
			++size;
			return;
		}
		else if (first_item - 1 < first_block)
		{
			T** aux = new T * [block_count + 1];
			int cnt = 0;
			for (int i = which_first_block; i < block_count + which_first_block; ++i)
			{
				aux[cnt + 1] = deq[i];
				++cnt;
			}
			aux[0] = new T[BLOCK_SIZE];
			delete[] deq;
			deq = aux;
			++size;
			++block_count;
			first_block = deq[0];
			first_block[BLOCK_SIZE - 1] = key;
			first_item = &(first_block[BLOCK_SIZE - 1]);
			which_first_block = 0;
			which_last_block = block_count - 1;
			return;
		}
		*(first_item - 1) = key;
		++size;
		--first_item;
	}

	void pop_back()
	{
		if (size == 0)
		{
			return;
		}
		else if (last_item == last_block)
		{
			delete[] last_block;

			--size;
			--block_count;
			--which_last_block;

			last_item = &(deq[which_last_block][BLOCK_SIZE - 1]);
			last_block = deq[which_last_block];
			return;
		}
		--last_item;
		--size;
	}
	void pop_front()
	{
		if (size == 0)
		{
			return;
		}
		else if (first_item == first_block + BLOCK_SIZE - 1)
		{
			delete[] first_block;
			--size;
			--block_count;
			++which_first_block;
			first_item = &(deq[which_first_block][0]);
			first_block = deq[which_first_block];
			return;
		}
		++first_item;
		--size;
	}
	void print()
	{
		/*for (int i = which_first_block; i < block_count + which_first_block; ++i)
		{
			for (int j = 0; j < BLOCK_SIZE; ++j)
			{
				std::cout << deq[i][j] << " ";
			}
		}*/
		if (!size)
		{
			std::cout << "The deque is empty!\n";
			return;
		}
		for (int i = 0; i < size; ++i)
			std::cout << *(at(i)) << " ";
		std::cout << '\n';
	}



	void insert(int pos, T key)
	{
		if (pos < 0 || pos > size)
		{
			std::cout << "Inserting position out of bounds! [ 0, " << size << " ]\n";
			return;
		}
		if (pos >= size / 2)
		{
			int start = size;

			push_back(*(last_item));

			for (int i = start; i >= pos; --i)
			{
				*(at(i)) = *(at(i - 1));
			}
			*(at(pos)) = key;
			return;
		}
		int start = 0;

		push_front(*(first_item));

		for (int i = start; i <= pos; ++i)
		{
			*(at(i)) = *(at(i + 1));
		}
		*(at(pos)) = key;
		return;
	}

	void remove(int pos)
	{
		if (pos >= size / 2)
		{
			int start = pos;
			for (int i = pos; i < size - 1; ++i)
			{
				*(at(i)) = *(at(i + 1));
			}
			pop_back();
			return;
		}
		int start = pos;
		for (int i = pos; i > 0; --i)
			*(at(i)) = *(at(i - 1));
		pop_front();
		return;
	}
	int dimension()
	{
		return size;
	}

	bool empty()
	{
		return (size == 0);
	}

	T& operator[](int i)
	{
		if (i < 0 || i >= size)
		{
			std::cout << "Index out of bounds!\n";
			return *(at(0));
		}
		return *(at(i));
	}

	void clear()
	{
		for (int i = which_first_block; i <= which_last_block; ++i)
			delete[] deq[i];
		delete[] deq;

		deq = new T * [1], deq[0] = new T[BLOCK_SIZE];
		last_block = first_block = deq[0];
		last_item = first_item = &deq[0][BLOCK_SIZE / 2];
		size = 0, block_count = 1;
		which_first_block = which_last_block = 0;
	}
};

void menu()
{
	std::cout << "Deque menu:\n" << "1 key - Push back key;\n";
	std::cout << "2 key - Push front key;\n";
	std::cout << "3 - Pop back;\n";
	std::cout << "4 - Pop front;\n";
	std::cout << "5 - Size;\n";
	std::cout << "6 - Is empty;\n";
	std::cout << "7 pos - Print the element from pos in deque;\n";
	std::cout << "8 pos key - Insert the key at pos in deque;\n";
	std::cout << "9 pos - Delete the element from pos;\n";
	std::cout << "10 - Clear the deque;\n";
	std::cout << "11 - Print the deque;\n";
	std::cout << "12 - Clear the screen;\n";
	std::cout << "0 - Stop.\n\n";
}
int main()
{
	deque<int> deq;

	menu();
	int x;
	int key, pos;
	std::cin >> x;
	while (x)
	{
		switch (x)
		{
		case 1:
			std::cin >> key;
			deq.push_back(key);
			break;
		case 2:
			std::cin >> key;
			deq.push_front(key);
			break;
		case 3:
			deq.pop_back();
			break;
		case 4:
			deq.pop_front();
		case 5:
			std::cout << "Deque has " << deq.dimension() << " elements.\n";
			break;
		case 6:
			if (deq.empty()) std::cout << "The deque is empty.\n";
			else std::cout << "The deque isn't empty.\n";
			break;
		case 7:
			std::cin >> pos;
			std::cout << deq[pos] << '\n';
			break;
		case 8:
			std::cin >> pos >> key;
			deq.insert(pos, key);
			break;
		case 9:
			std::cin >> pos;
			deq.remove(pos);
			break;
		case 10:
			deq.clear();
			break;
		case 11:
			deq.print();
			break;
		case 12:
			system("CLS");
			menu();
			break;
		}
		std::cin >> x;
	}
	deq.clear();
	return 0;
}