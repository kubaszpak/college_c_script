#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <math.h>

struct complex
{
	float ReZ;
	float ImZ;
};

struct complex new_complex(float ReZ, float ImZ)
{
	struct complex new = {ReZ, ImZ};
	return new;
}

struct element
{
	struct complex current;
	struct element *previous;
};

void print_complex(struct complex complex);

struct element *create_stack()
{
	struct element *top = malloc(sizeof(struct element));
	struct complex base;
	base.ReZ = 0;
	base.ImZ = 0;
	top->current = base;
	top->previous = NULL;
	return top;
}

bool stack_empty(struct element *top)
{
	// printf("Checking...");
	if (top == NULL)
	{
		return true;
	}
	if (top->previous == NULL)
	{
		// printf("Stack is empty\n");
		printf("================================\n");
		return true;
	}
	else
	{
		// printf("Stack is not empty\n");
		printf("--------------------------------\n");
		return false;
	}
}

void print_stack(struct element *top)
{
	printf("\nPrinting stack from the top element\n\n");
	while (!stack_empty(top))
	{
		print_complex(top->current);
		top = top->previous;
	}
	return;
}

void print_complex(struct complex complex)
{
	if (complex.ImZ >= 0)
	{
		printf("Complex: %.3f + %.3fi\n", complex.ReZ, complex.ImZ);
	}
	else
	{
		printf("Complex: %.3f %.3fi\n", complex.ReZ, complex.ImZ);
	}
}

void free_memory(struct element *top)
{
	while (!stack_empty(top))
	{
		printf("Deleting an element...\n");
		struct element *elem = top->previous;
		free(top);
		top = elem;
	}
	return;
}

void stack_push(struct element **top, struct complex new_complex)
{
	struct element *new_element = malloc(sizeof(struct element));
	new_element->current = new_complex;
	new_element->previous = *top;
	// printf("%d\n", new_element);
	*top = new_element;
	return;
}

struct complex stack_pop(struct element **top)
{
	assert(*top != NULL);
	assert(top != NULL);
	assert(!stack_empty(*top));
	struct complex bufor = (*top)->current;
	struct element *temp = (*top)->previous;
	free(*top);
	*top = temp;
	return bufor;
}

struct complex add_two_complex_numbers(struct complex complex1, struct complex complex2)
{
	struct complex new = {complex2.ReZ + complex1.ReZ, complex2.ImZ + complex1.ImZ};
	return new;
}

struct complex substract_two_complex_numbers(struct complex complex1, struct complex complex2)
{
	// complex2 została zdjęta ze stosu jako druga, więc była jako pierwsza do niego dodana
	// co oznacza że ona jest odjemną i stoi jako pierwsza w działaniu
	struct complex new = {complex2.ReZ - complex1.ReZ, complex2.ImZ - complex1.ImZ};
	return new;
}

struct complex multiply_two_complex_numbers(struct complex complex1, struct complex complex2)
{
	struct complex new = {complex2.ReZ * complex1.ReZ - complex2.ImZ * complex1.ImZ, complex2.ReZ * complex1.ImZ + complex1.ReZ * complex2.ImZ};
	return new;
}

struct complex divide_two_complex_numbers(struct complex complex1, struct complex complex2)
{
	struct complex new;
	if (abs(complex1.ImZ) < abs(complex1.ReZ))
	{
		float doc = complex1.ImZ / complex1.ReZ;
		new.ReZ = (complex2.ReZ + complex2.ImZ * doc) / (complex1.ReZ + complex1.ImZ * doc);
		new.ImZ = (complex2.ImZ - complex2.ReZ * doc) / (complex1.ReZ + complex1.ImZ * doc);
	}
	else
	{
		float cod = complex1.ReZ / complex1.ImZ;
		new.ReZ = (complex2.ImZ + complex2.ReZ * cod) / (complex1.ImZ + complex1.ReZ * cod);
		new.ImZ = ((-1) * complex2.ReZ + complex2.ImZ * cod) / (complex1.ImZ + complex1.ReZ * cod);
	}
	return new;
}

int main()
{
	struct element *top = create_stack();
	while (true)
	{
		printf("\n\nEnter either: 1 or 2 floating point numbers for real and imaginary parts of a complex number, \none of these operators +,-,/,* or e to exit:\n\n");
		char array[100];
		fgets(array, 100, stdin);
		float ReZ;
		float ImZ;
		char operator[10];
		char rubbish[100];
		int output_check = sscanf(array, "%f %s", &ReZ, rubbish);
		int output_check2 = sscanf(array, "%f %f %s", &ReZ, &ImZ, rubbish);
		if (output_check == 0 && output_check2 == 0)
		{
			int output2 = sscanf(array, "%s %s", operator, rubbish);

			switch (output2)
			{
			case 0:
				printf("Case 0");
				break;
			case 1:
				printf("Case 1 - operator: %s\n", operator);
				if (strcmp(operator, "e") == 0)
				{
					printf("Exiting...\n\n");
					free_memory(top);
					return 0;
				}
				else if (strcmp(operator, "+") == 0)
				{
					if (!stack_empty(top->previous))
					{
						struct complex result = add_two_complex_numbers(stack_pop(&top), stack_pop(&top));
						printf("\nAdding the result to the stack\n");
						print_complex(result);
						printf("\n");
						stack_push(&top, result);
					}
					else
					{
						printf("There have to be at least two numbers on the stack to perform this operation\n\n");
					}
				}
				else if (strcmp(operator, "-") == 0)
				{
					if (!stack_empty(top->previous))
					{
						struct complex last = stack_pop(&top);
						struct complex previous = stack_pop(&top);
						struct complex result = substract_two_complex_numbers(last, previous);
						printf("\nAdding the result to the stack\n");
						print_complex(result);
						printf("\n");
						stack_push(&top, result);
					}
					else
					{
						printf("There have to be at least two numbers on the stack to perform this operation\n\n");
					}
				}
				else if (strcmp(operator, "*") == 0)
				{
					if (!stack_empty(top->previous))
					{
						struct complex last = stack_pop(&top);
						struct complex previous = stack_pop(&top);
						struct complex result = multiply_two_complex_numbers(last, previous);
						printf("\nAdding the result to the stack\n");
						print_complex(result);
						printf("\n");
						stack_push(&top, result);
					}
					else
					{
						printf("There have to be at least two numbers on the stack to perform this operation\n\n");
					}
				}
				else if (strcmp(operator, "/") == 0)
				{

					if (!stack_empty(top->previous))
					{
						struct complex last = stack_pop(&top);
						if (last.ReZ == 0 && last.ImZ == 0)
						{
							printf("Division by zero is not allowed ... try a different operation or add a new number\n");
						}
						else
						{
							struct complex result = divide_two_complex_numbers(last, stack_pop(&top));
							printf("\nAdding the result to the stack\n");
							print_complex(result);
							printf("\n");
							stack_push(&top, result);
						}
					}
					else
					{
						printf("There have to be at least two numbers on the stack to perform this operation\n\n");
					}
				}
				else
				{
					printf("Unknown operator, going back to the beginning...\n");
				}
				break;
			case 2:
				printf("Invalid input... you can only pass one operator at one point\n");
				break;
			deafult:
				printf("Invalid input, going back to the beginning...\n");
				break;
			}
		}
		else if (output_check == 1 && output_check2 == 1)
		{
			stack_push(&top, new_complex(ReZ, 0));
		}
		else if (output_check2 == 2)
		{
			stack_push(&top, new_complex(ReZ, ImZ));
		}
		else
		{
			printf("\nError, invalid input ... \nTry either 1 or 2 floating point numbers(seperated by a space) for real and imaginary parts of a complex number, \none of these operators +,-,/,* or e to exit::\n\n");
		}
		print_stack(top);
	}
	return 0;
}