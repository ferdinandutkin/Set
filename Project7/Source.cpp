#include <iostream>
#include <vector>
#include <type_traits>
#include <algorithm>
 



template<typename T>
constexpr bool is_pair_v{};


template<typename T, typename U>
constexpr bool is_pair_v<std::pair<T, U>> = true;
 

template <typename T>
class set {
private:	
	std::vector<T> container;
public:
	template<typename... Ts>
	set(Ts... args) {
		(append(args),...);
	}
	
	set() = default;


	void append (T el) {
	
		if (auto it = std::find_if_not(container.begin(), container.end(),
			[&](T c_el) {
				return c_el < el;
			});
			it == container.end() or *it != el) {
			container.insert(it, el);
		}
		

	}

	template <typename U>
	set<std::pair<T, U>> cartesian_product(const set<U>& r) {
		set<std::pair<T, U>> ret;
		for (T i : container) {
			for (U j : r.container) {
				ret.append(std::make_pair(i, j));
			}
		}
		return ret;

	}

	bool operator==(set& r) {
		return power() == r.power() and std::equal(begin(), end(), r.begin());
	}
	bool operator<(set& r) {
		return power() < r.power();
	}

	bool operator>(set& r) {
		return power() > r.power();
	}

	friend std::ostream& operator << (std::ostream& os, const set& s) {
			os << '{';
			for (const T& el : s) {
				os << el;
				if (el != s.container.back()) {
					os << ", ";
				}
			}
			os << '}';

		return os;
		
	}


	template <typename U> //шота сложно и 	set<typename T::second_type> не вышло
	bool is_surjective(set<U>& Y) const
		requires is_pair_v<T> and std::is_same_v<typename T::second_type, U>  { //лишние реквайрз)

		set<typename T::second_type> p2;
		

		for (auto&& [x, y] : container)
			p2.append(y);

		return Y == p2;
	}


	template <typename U>
	bool is_everywhere_defined(set<U>& X) const
		requires is_pair_v<T> and std::is_same_v<typename T::first_type, U> {


	
		set<U> p1;
 

		for (auto&& [x, y] : container) {
			p1.append(x);
		}
			

		return X == p1;
	}


	template <typename U, typename J>
	bool is_injective(set<U>& X, set<J>& Y) const
		requires is_pair_v<T> and std::is_same_v<typename T::first_type, U>
		and std::is_same_v<typename T::second_type, J> {

		set<U> p1;
		set<J> p2;

		for (auto [x, y] : container) {
			p1.append(x);
			p2.append(y);
		}
		//кол-во жлтов соотв множеств НЕ меньше чем у соответсивия	

		return power() <= std::min(p1.power(), p2.power());
	}



	template <typename U, typename J>
	bool is_bijective(set<U> &X, set<J>& Y) const
		requires is_pair_v<T>and std::is_same_v<typename T::first_type, U>{

		return is_injective(X, Y) and is_surjective(Y);

	}



	size_t power() const {
		return container.size();
	}

	auto begin() const {
		return container.begin();
	}
	auto end() const {
		return container.end();
	}
	

	const T operator [] (const size_t index) const {
		return container.at(index);
	}


};



template<typename T>
class subsets_iter {
	set<T> owner;
public:
	subsets_iter(const set<T>& owner) : owner{ owner } { }

	class iter {
		set<T> owner;
		size_t current;
	public:

		iter(const set<T>& owner, const size_t current = 0) : current{ current }, owner{ owner } {}

		iter& operator++() {
			current++;
			return *this;
		}

		iter operator++(int) {
			iter temp{ *this };
			operator++();
		    return temp;
	
		}

		bool operator != (const iter& r) {
			return current != r.current;
		}
	

		set<T> operator*() {
			set<T> s;
			for (size_t cur = current; const T& el : owner) {
				if (cur & 1) {
					s.append(el);
				}
				cur >>= 1;
			}
			return s;
		}


	};
	

	auto begin() const {
		return iter{ owner };
	}

	auto end() const {
		return iter{ owner, 1u << owner.power() };
	}
	 
};




 


template<typename T, typename U>

std::ostream& operator << ( std::ostream& os, const std::pair<T, U> pair) {
	return os << '(' << pair.first << ", " << pair.second << ')';

}




int main(int argc, char* argv[]) {


	set<int> X = { 1, 2};
	set<int> Y = { 1, 2};

	

 

 

 

	
	for (auto subset : subsets_iter{ X.cartesian_product(Y) }) {


		if (subset.is_bijective(X, Y)) {
			std::cout << subset << std::endl;
		}
	   std::cout << subset << std::endl;
	   std::cout << "Surjective: " << subset.is_surjective(Y) << ".  ";
	   std::cout << "Is injective " << subset.is_injective(X, Y) << std::endl;
	   std::cout << "Is everywhere defined: " << subset.is_everywhere_defined(X) << std::endl << std::endl;


	}
	
	 

 
	if (argc > 1) {

		 
		set<int> args;
		for (int i{ 1 }; i < argc; i++) {
			args.append(atoi(argv[i]));
		}

		for (int i{}; auto sub : subsets_iter{ args }) {

			std::cout << ++i << ") " << sub << std::endl;

		}

	}
	/*
	set<int> set = { 1, 2, 3, 3, 4 , 13 };

	for (int i{}; auto sub : subsets{ set }) {

		std::cout << ++i << ") " << sub << std::endl;

	}
	*/
	
}