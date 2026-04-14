#ifndef BPT_MEMORYRIVER_HPP
#define BPT_MEMORYRIVER_HPP

#include <fstream>
#include <string>

using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;

template<class T, int info_len = 2>
class MemoryRiver {
private:
    fstream file;
    string file_name;
    int sizeofT = sizeof(T);

    void open_file() {
        file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
    }

    void close_file() {
        if (file.is_open()) {
            file.close();
        }
    }

public:
    MemoryRiver() = default;

    MemoryRiver(const string& file_name) : file_name(file_name) {}

    void initialise(string FN = "") {
        if (FN != "") file_name = FN;
        file.open(file_name, std::ios::out | std::ios::binary);
        int tmp = 0;
        for (int i = 0; i < info_len + 1; ++i)
            file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
    }

    //读出第n个int的值赋给tmp，1_base
    void get_info(int &tmp, int n) {
        if (n > info_len || n <= 0) return;
        open_file();
        file.seekg((n - 1) * sizeof(int));
        file.read(reinterpret_cast<char *>(&tmp), sizeof(int));
        close_file();
    }

    //将tmp写入第n个int的位置，1_base
    void write_info(int tmp, int n) {
        if (n > info_len || n <= 0) return;
        open_file();
        file.seekp((n - 1) * sizeof(int));
        file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        close_file();
    }
    
    //在文件合适位置写入类对象t，并返回写入的位置索引index
    int write(T &t) {
        open_file();
        int head;
        file.seekg(info_len * sizeof(int));
        file.read(reinterpret_cast<char *>(&head), sizeof(int));

        int index;
        if (head == 0) {
            file.seekp(0, std::ios::end);
            index = file.tellp();
            file.write(reinterpret_cast<char *>(&t), sizeofT);
        } else {
            index = head;
            int next_head;
            file.seekg(head);
            file.read(reinterpret_cast<char *>(&next_head), sizeof(int));
            
            file.seekp(info_len * sizeof(int));
            file.write(reinterpret_cast<char *>(&next_head), sizeof(int));

            file.seekp(index);
            file.write(reinterpret_cast<char *>(&t), sizeofT);
        }
        close_file();
        return index;
    }
    
    //用t的值更新位置索引index对应的对象
    void update(T &t, const int index) {
        open_file();
        file.seekp(index);
        file.write(reinterpret_cast<char *>(&t), sizeofT);
        close_file();
    }
    
    //读出位置索引index对应的T对象的值并赋值给t
    void read(T &t, const int index) {
        open_file();
        file.seekg(index);
        file.read(reinterpret_cast<char *>(&t), sizeofT);
        close_file();
    }
    
    //删除位置索引index对应的对象
    void Delete(int index) {
        open_file();
        int head;
        file.seekg(info_len * sizeof(int));
        file.read(reinterpret_cast<char *>(&head), sizeof(int));

        file.seekp(index);
        file.write(reinterpret_cast<char *>(&head), sizeof(int));

        file.seekp(info_len * sizeof(int));
        file.write(reinterpret_cast<char *>(&index), sizeof(int));
        close_file();
    }
};

#endif //BPT_MEMORYRIVER_HPP
