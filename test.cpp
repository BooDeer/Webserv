#include <iostream>
#include <unistd.h>

class foo
{
	public:
    int t1;
    int t2;
	foo(): t1(10), t2(20){}
};

void	test2()
{
	int test;
	std::cout << test<< std::endl;
}
void	test1()
{
	int test;
	std::cout << test<< std::endl;
	test2();
}
struct abc
{
	int i;
	int b;
	abc(): i(10), b(20) {}
};
int main(int ac, char **av)
{
	foo t;
	// int test;
	// std::cout << test<< std::endl;
	// std::cout << t.t << std::endl;
	// test1();
	abc test;
	int ibc =  '(int')((double ')(&A)++);
	std::cout << ibc << std::endl;












}