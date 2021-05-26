#include <initializer_list>
#include <ostream>

namespace mancala {
class Pockets {
	struct Pocket {
		int marbles;
		Pocket* next;

		Pocket(int m, Pocket* n): marbles(m), next(n) {}
	};
	/*
	class PocketIterator {
	public:
		using value_type = Pockets::value_type;
		using size_type = Pockets::size_type;

		PocketIterator(): p{nullptr} {}
		PocketIterator(Pocket* ptr, size_type i = 0): p{ptr}, {}

		size_type index() const noexcept { return idx_; }
		value_type& operator*() const noexcept { return p->marbles; }

		PocketIterator& operator++() const noexcept {
			p = p->next;
			++idx_;
			if (p == head) idx_ = 0;
			return *this;
		}

		bool operator==(const PocketIterator& rhs) const noexcept {
			return p == rhs.p;
		}
		bool operator!=(const PocketIterator& rhs) const noexcept {
			return p != rhs.p;
		}

	private:
		Pocket* p;
		size_type idx_;
	};*/

public:
	using size_type = std::size_t;
	using value_type = int;
	using iterator = void;  // TODO: Write iterator class

	Pockets();
	Pockets(size_type count, const value_type& value = value_type());
	Pockets(std::initializer_list<value_type> init);
	~Pockets() noexcept;

	Pockets(const Pockets&);
	Pockets& operator=(const Pockets&);
	Pockets(Pockets&&) noexcept;
	Pockets& operator=(Pockets&&) noexcept;

	void append(const value_type&);
	void insert(size_type, const value_type&);
	void remove(size_type);
	void clear();

	size_type size() const noexcept { return size_; }

	value_type operator[](size_type n) const noexcept { return at(n); }
	value_type& operator[](size_type n) noexcept { return at(n); }

	value_type at(size_type) const noexcept;
	value_type& at(size_type) noexcept;

	iterator begin() const noexcept;
	iterator end() const noexcept;

	bool operator==(const Pockets&) const noexcept;
	bool operator<(const Pockets&) const noexcept;

	// NYI
	// some_type serialize() const;

protected:
	friend std::ostream& operator<<(std::ostream&, const Pockets&);

	// Insert after p or at head if p is nullptr.
	void insert_after(Pocket* p, const value_type& value) {
		if (p) p->next = new Pocket(value, p->next);
		else {
			Pocket* tail = get_tail();
			head = new Pocket(value, head);
			if (tail)
				tail->next = head;
			else
				head->next = head;
		}
		++size_;
	}

	/**
	* Get the pointer before head.
	* 
	* @return nullptr if head is nullptr, or the pointer before head.
	**/
	Pocket* get_tail() const noexcept;

private:
	Pocket* head{nullptr};
	size_type size_{0};
};
}  // namespace mancala
