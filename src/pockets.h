#include <initializer_list>
#include <ostream>

namespace mancala {
class Pockets {
	struct Pocket {
		int marbles;
		Pocket* next;

		Pocket(int m, Pocket* n): marbles(m), next(n) {}
	};

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

private:
	Pocket* head{nullptr};
	size_type size_{0};
};
}  // namespace mancala
