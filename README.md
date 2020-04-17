# version3.0
with complete database

Configuration:

1. Put 'index.html', image and css file into htdocs folder of Apache
2. Open terminal and navigate to cgi_main
3. Enter compile instruction 'g++ Main.cpp func_main.cpp -std=c++11 -o cgi_main.cgi'
4. Put 'cgi_main.cgi' and src file into cgi-bin folder of Apache
5. Navigate path to cgi_other
6. Enter compile instruction 'g++ website_answer.cpp -std=c++11 -o website_answer.cgi'
7. Enter compile instruction 'g++ random_question.cpp -std=c++11 -o random_question.cgi'
8. Put 'website_answer.cgi' and 'random_question.cgi' into cgi-bin folder of Apache
