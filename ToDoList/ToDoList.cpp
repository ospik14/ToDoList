#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#endif

bool cinCheck(const int &choice, int maxChoice) {
	if (std::cin.fail() || choice < 1 || choice > maxChoice) {
		std::cin.clear();
		std::cin.ignore(32767, '\n');
		std::cout << "Вибір не коректний, спробуйте ще раз" << std::endl;
		return true;
	}
	else
		return false;
}

class Task {
private:
	std::string task;
	bool status;

public:
	void setTask(std::string t) { task = t; }
	void setStatus(bool s) { status = s; }

	std::string getTask() { return task; }
	bool getStatus() { return status; }

	Task(std::string t, bool s) {
		setTask(t);
		setStatus(s);
	}

	void viewTasks(int i) const {
		if (status)
			std::cout << "[V] ";
		else
			std::cout << "[X] ";
		std::cout << i + 1 << ". " << task << std::endl;
	}
};

class App {
private:
	std::vector<Task> taskslist;

public:
	bool isExitToMenu() {
		int choice;
		do {
			std::cout << "Ваш вибір: ";
			std::cin >> choice;
		} while (cinCheck(choice, 2));
		if (choice == 1)
			return true;
		else
			return false;
	}

	void menu(int& choice) {
		using namespace std;

		cout << "--------- ГОЛОВНЕ МЕНЮ ----------" << endl;
		cout << "1. Додати завдання" << endl;
		cout << "2. Переглянути всі завдання" << endl;
		cout << "3. Змінити статус завдання" << endl;
		cout << "4. Редагувати завдання" << endl;
		cout << "5. Вийти" << endl;
		do {
			cout << "Ваш вибір: ";
			cin >> choice;
		} while (cinCheck(choice, 5));
	}

	void addTask() {
		std::cout << "Введіть завдання: " << std::endl;
		std::string task;
		std::cin.ignore(32767, '\n');
		getline(std::cin, task);

		if (!task.empty()) {
			taskslist.push_back(Task(task, false));
			std::cout << "Завдання додано!" << std::endl;
		}
		else
			std::cout << "Ви нічого не ввели" << std::endl;

	}

	void printTasks() {
		std::cout << "----- Ваші завдання -----" << std::endl;
		for (int i = 0; i < taskslist.size(); i++) {
			taskslist.at(i).viewTasks(i);
		}
		std::cout << "-------------------------" << std::endl;
	}

	void changeStatus() {
		std::cout << "Виберіть виконане завдання: " << std::endl;
		printTasks();
		int choice;
		do {
			std::cout << "Ваш вибір: ";
			std::cin >> choice;
		} while (cinCheck(choice, taskslist.size()));
		
		taskslist.at(choice - 1).setStatus(true);
		std::cout << "Завдання позначено як виконане" << std::endl;
	}

	void changeTask() {
		std::cout << "Виберіть завдання для редагування: " << std::endl;
		printTasks();
		int choice;
		do {
			std::cout << "Ваш вибір: ";
			std::cin >> choice;
		} while (cinCheck(choice, taskslist.size()));

		std::cout << "Введіть відредаговане завдання: " << std::endl;
		std::string changeTask;
		std::cin.ignore(32767, '\n');
		getline(std::cin, changeTask);

		if (!changeTask.empty()) {
			taskslist.at(choice - 1).setTask(changeTask);
			std::cout << "Завдання змінено!" << std::endl;
		}
		else
			std::cout << "Ви нічого не змінили" << std::endl;
	}

	void readFile() {
		std::ifstream file("Tasks.txt");
		if (file.is_open()) {
			std::string line, segment;

			while (std::getline(file, line)) {
				std::stringstream ss(line);

				std::string task;
				bool status;

				std::getline(ss, segment, ' ');
				std::getline(ss, task, ' ');

				if (segment[0] == '0') status = false;
				if (segment[0] == '1') status = true;

				taskslist.push_back(Task(task, status));
			}
			file.close();
		}
	}

	void saveInFile() {
		std::ofstream file("Tasks.txt");
		if (file.is_open()) {
			for (int i = 0; i < taskslist.size(); i++) {
				file << taskslist.at(i).getStatus() << " " << taskslist.at(i).getTask() << std::endl;
			}
		}
	}

	void run() {
		int choice;
		bool isRun = true; 
		readFile();

		while (isRun) {
			system("cls");
			menu(choice);
			switch (choice) {
			case 1: {
				system("cls");
				addTask();
				std::cout << "1 - Вийти в головне меню \n2 - Додати ще завдання" << std::endl;
				if (isExitToMenu()) break;
				else addTask(); break;
			}
			case 2: {
				system("cls");
				if (!taskslist.empty())
					printTasks();
				else
					std::cout << "Cписок порожній" << std::endl;

				std::cout << "1 - Вийти в головне меню \n2 - Додати завдання" << std::endl;
				if (isExitToMenu()) break;
				else addTask(); break;
			}
			case 3: {
				system("cls");
				changeStatus();
				std::cout << "1 - Вийти в головне меню \n2 - Змінити статус завдання" << std::endl;
				if (isExitToMenu()) break;
				else changeStatus(); break;
			}
				  
			case 4: {
				system("cls");
				changeTask();
				std::cout << "1 - Вийти в головне меню \n2 - Відредагувати завдання" << std::endl;
				if (isExitToMenu()) break;
				else changeTask(); break;
			}

			case 5: {
				saveInFile();
				isRun = false;
			}
				
			}
		}
	}
};


int main() {
	#ifdef _WIN32
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	#endif
	
	App taskslist;
	taskslist.run();

	return 0;
}