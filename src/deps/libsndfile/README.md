# libsndfile

* Need to move the loadSound function from OpenALSound to a new class
* Reason? I like to group code together that calls upon a specific dependency
* As an example, look at src/deps/rapidjson, you'll see all of the code in the entire project that references rapidjson there
