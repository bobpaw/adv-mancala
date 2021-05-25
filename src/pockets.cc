#include <algorithm>
#include <stdexcept>

#include "pockets.h"

namespace mancala {
Pockets::Pockets(): size_(0) {}

Pockets::Pockets(size_type count, const value_type& value) {
	for (size_type i = 0; i != count; ++i) append(value);
}

Pockets::Pockets(std::initializer_list<value_type> init) {
	for (auto v : init) append(v);
}

Pockets::~Pockets() noexcept { clear(); }

// Reimplement to go by element pointer
Pockets::Pockets(const Pockets& old) {
	for (size_type i = 0; i != old.size(); ++i) append(old[i]);
}

// TODO: Write me
Pockets& Pockets::operator=(const Pockets& old) {
	for (size_type i = 0; i != size_; ++i) at(i) = old.at(i);
	if (size_ < old.size_) {
		for (size_type i = size_; i != old.size_; ++i) append(old[i]);
	}

	return *this;
}

Pockets::Pockets(Pockets&& old) noexcept: head(old.head), size_(old.size_) {
	old.head = nullptr;
	old.size_ = 0;
}

Pockets& Pockets::operator=(Pockets&& old) noexcept {
	clear();

	head = old.head;
	size_ = old.size_;

	old.head = nullptr;
	old.size_ = 0;

	return *this;
}

void Pockets::append(const value_type& value) {
	if (head) {
		Pocket* current = head;
		while (current->next != head) current = current->next;
		current->next = new Pocket(value, head);
	} else {
		head = new Pocket(value, nullptr);
		head->next = head;
	}

	++size_;
}

void Pockets::insert(size_type pos, const value_type& value) {
	// FIXME: Doesn't allow insertion at 0 of a size 0 list.
	if (!(pos < size_)) throw std::out_of_range("Pockets::insert out of range");

	Pocket* current = head;
	for (size_type i = 0; i != pos; ++i) current = current->next;
	current->next = new Pocket(value, current->next);
	std::swap(current->marbles, current->next->marbles);

	++size_;
}

void Pockets::remove(size_type pos) {
	if (!(pos < size_)) throw std::out_of_range("Pockets::remove out of range");

	Pocket *parent = nullptr, *current = head;

	for (size_type i = 0; i != pos; ++i) {
		parent = current;
		current = current->next;
	}

	if (parent) {
		Pocket* hook = parent->next;
		parent->next = hook->next;
		delete hook;
	} else {
		// Delete head
		if (size_ == 1) {
			delete head;
			head = nullptr;
		} else {
			Pocket* tail = head;
			while (tail->next != head) tail = tail->next;

			tail->next = head->next;
			delete head;
			head = tail->next;
		}
	}

	--size_;
}

void Pockets::clear() {
	// FIXME: Make it less slow by going through pointers
	while (size_ != 0) remove(0);
}

Pockets::value_type Pockets::at(size_type n) const noexcept {
	Pocket* p = head;

	for (size_type i = 0; i != n; ++i) p = p->next;

	return p->marbles;
}

Pockets::value_type& Pockets::at(size_type n) noexcept {
	Pocket* p = head;

	for (size_type i = 0; i != n; ++i) p = p->next;

	return p->marbles;
}

// FIXME: Implement
Pockets::iterator Pockets::begin() const noexcept {}
Pockets::iterator Pockets::end() const noexcept {}

bool Pockets::operator==(const Pockets& rhs) const noexcept {
	if (size_ != rhs.size_) return false;

	if (head->marbles != rhs.head->marbles) return false;

	// FIXME: Re-implement to make less slow by going through pointers
	for (size_type i = 0; i != size_; ++i)
		if (at(i) != rhs[i]) return false;
	return true;
}

bool Pockets::operator<(const Pockets& rhs) const noexcept {
	size_type minsize = std::min(size_, rhs.size_);

	for (size_type i = 0; i < minsize; ++i)
		if (at(i) < rhs[i])
			return true;
		else if (rhs[i] < at(i))
			return false;

	return size_ < rhs.size_;
}

std::ostream& operator<<(std::ostream& out, const Pockets& p) {
	Pockets::Pocket* pp = p.head;
	for (Pockets::size_type i = 0; i != p.size() - 1; pp = pp->next, ++i) {
		out << pp->marbles << ' ';
	}

	out << pp->marbles;

	return out;
}
}  // namespace mancala
