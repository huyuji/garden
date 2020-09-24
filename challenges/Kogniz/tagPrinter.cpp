/*
* The code below prints the tags of an identified person.
* The bits of the tags property indicate the indexes of the tags in the tag list.
*/
vector<string> tag_types;
tag_types.push_back("Known");
tag_types.push_back("VIP");
tag_types.push_back("Suspect");
tag_types.push_back("Employee");
...
tag_types.push_back("Friend");
// tag_types.size() == 200
/*
* It is slightly faster to initialize the vector using the initializer list:
* vector<string> tag_types {"Known", "VIP", ...}
*/
...
try {
	auto p = g_recognizer.identify_person(inf);
	if (!p) {
		/*
		* It seems this exception is supposed to be handled by the caller.
		* In that case it is better to throw a std::exception or more specific type of exception.
		*/
		throw "Person not identified";
	}
	for (unsigned i = 0; i < tag_types.size(); ++i) {
		/*
		* Since tag_types.size() is 200, uint64_t(pow(2, i)) will clearly overflow.
		* We can use a std::bitset or 4 uint64_t to contain all the possible bits.
		* Also (2 << i) would be much more efficient and readable than pow(2, i).
		*/
		if (p.tags & uint64_t(pow(2, i))) {
			/*
			* cout << '\n' prints the same and is supposed to be much faster than cout << endl
			*/
			cout << tag_types[i] << endl;
		}
	}
}
catch (const exception& e) {
	...
}