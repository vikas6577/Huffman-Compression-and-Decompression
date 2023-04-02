#include <bits/stdc++.h>
using namespace std;
#define runfast                       \
    ios_base::sync_with_stdio(false); \
    cin.tie(NULL);                    \
    cout.tie(NULL)

int log_sz_of_tree = 2;

// void pre()
// {
//     int temp = 2;
//     while (temp >>= 1)
//         ++log_sz_of_tree;
// }

class Node
{
public:
    Node *next[2];
    int freq;
    char ch;
    bool leaf;
    Node()
    {
        for (int i = 0; i < 2; i++)
            next[i] = NULL;
        freq = 0;
        leaf = 1;
    }
    Node(char ch, int freq)
    {
        this->ch = ch;
        this->freq = freq;
        for (int i = 0; i < 2; i++)
            next[i] = NULL;
        leaf = 1;
    }
    Node(int freq, vector<Node *> &vect)
    {
        this->freq = freq;
        for (int i = 0; i < vect.size(); i++)
            next[i] = vect[i];
        leaf = 0;
    }
};
void set_file_type(string original, string &name, string &type)
{
    bool check = 1;
    int n = original.size();
    for (int i = 0; i < n; i++)
    {
        if (original[i] == '.')
            check = 0;
        else
        {
            if (check)
                name.push_back(original[i]);
            else
                type.push_back(original[i]);
        }
    }
}

void write(FILE *output, string s)
{
    int n = s.size();
    fwrite(s.c_str(), 1, n, output);
}

string decimal_to_binary(int n)
{
    string ans = "";
    while (n > 0)
    {
        if (n & 1)
            ans.push_back('1');
        else
            ans.push_back('0');
        n >>= 1;
    }

    while (ans.size() < log_sz_of_tree)
        ans.push_back('0');
    reverse(ans.begin(), ans.end());
    return ans;
}

void dfsSearch(Node *&root, string &s, unordered_map<char, string> &m)
{
    if (root->leaf)
        m[root->ch] = s;
    else
    {
        for (int i = 0; i < 2; i++)
        {
            if (root->next[i] == NULL)
                continue;
            s += decimal_to_binary(i);
            dfsSearch(root->next[i], s, m);
            for (int j = 0; j < log_sz_of_tree; j++)
                s.pop_back();
        }
    }
}

int main(int argc, char *argv[])
{
    runfast;

    // Opening the input provided file using command line argument in read mode
    FILE *input_file = fopen(argv[1], "r");
    if (input_file == NULL)
    {
        cout << "Input File can't be Opened" << endl;
        return 0;
    }
    else
    {
        string input_file_name = "", input_file_type = "";
        // variables to store the input file name and its type in seperate variable as type name will be used in decompression
        set_file_type(argv[1], input_file_name, input_file_type);
        // defining the name of compressed file,it will be same as the input file name just with compressed written in end
        string output_argument = input_file_name + "_compressed.bin";

        FILE *output_file = fopen(output_argument.c_str(), "wb");
        if (output_file == NULL)
        {
            cout << "Output File can't be Opened" << endl;
            return 0;
        }

        //    Start the compressed file by writing the size of file type and the file type for decompression
        write(output_file, to_string(input_file_type.size()) + input_file_type);

        // take the complete data of input file in a string
        string input_data_char = "", input_data_bin = "";
        char chararray[1];

        // find the frequency of all the characters present in the input file
        unordered_map<char, int> frequency;
        while (fread(chararray, 1, 1, input_file))
        {
            frequency[chararray[0]]++;
            input_data_char.push_back(chararray[0]);
        }

        // closing the file to release all the resources
        fclose(input_file);

        priority_queue<pair<int, Node *>, vector<pair<int, Node *>>, greater<pair<int, Node *>>> min_pq;

        // new variable length codes for all characters will be created and stored in a map
        unordered_map<char, string> code_to_string_mapping;
        Node *root;
        for (auto itr : frequency)
        {
            Node *temp = new Node(itr.first, itr.second);

            // adding all the Nodes in the minimum heap (priority queue)
            min_pq.push({itr.second, temp});
        }
        while (min_pq.size() > 0)
        {
            // when we have find the root node
            if (min_pq.size() == 1)
            {
                root = min_pq.top().second;
                min_pq.pop();
                break;
            }
            vector<Node *> Node_pack;
            int total_frequecies = 0;

            while (min_pq.size() > 0 && Node_pack.size() < 2)
            {
                Node *curr_Node = min_pq.top().second;
                int curr_freq = min_pq.top().first;
                min_pq.pop();
                total_frequecies += curr_freq;
                Node_pack.push_back(curr_Node);
            }
            Node *temp = new Node(total_frequecies, Node_pack);
            min_pq.push({total_frequecies, temp});
        }

        string s = "";
        // finding all the code to string mapping
        dfsSearch(root, s, code_to_string_mapping);

        // taking the data in the input_data_bin in binary
        for (int i = 0; i < input_data_char.size(); i++)
            input_data_bin += code_to_string_mapping[input_data_char[i]];

        // padding is added to the input data to make it divisible by 8, so that we can take 1 byte(8 bits) at a time

        int padding = (input_data_bin.size()) % 8;
        while ((input_data_bin.size()) % 8)
        {
            input_data_bin.push_back('\0');
        }

        for (auto itr : code_to_string_mapping)
        {
            string code = itr.first + itr.second;
            fwrite(code.c_str(), code.size() + 1, 1, output_file);
            // code.size()+1  is used so that NULL character is in the end, with the help of NULL character we can seperate different characters and their codes
            // code.c_str (it is used to convert string to character array, if we want to use fwrite function than it is applied on character array )
        }

        // write one more Null charcter so we know codes are ended
        // one Extra NULL character is added so that we know that codes are ended now padding will be the next output
        char null_char = '\0';
        fwrite(&null_char, 1, 1, output_file);

        // padding is written in the file
        char padding_str = padding + '0';
        fwrite(&padding_str, 1, 1, output_file);

        // &padding_str: This is a pointer to the data that you want to write to the file. In this case, it is a pointer to a variable named padding_str.

        // 1: This is the size of each element you want to write, in bytes. In this case, it is 1 byte.

        // 1: This is the number of elements you want to write. In this case, it is 1 element.

        // output_file: This is a pointer to the file you want to write to. In this case, it is a pointer to a file named output_file.

        // null
        fwrite(&null_char, 1, 1, output_file);

        // write the whole file
        for (int i = 0; i < input_data_bin.size(); i += 8)
        {
            int bit_pos = 7;
            unsigned char byte = 0b00000000;
            for (int j = i; j < i + 8; j++, bit_pos--)
            {
                if (input_data_bin[j] == '1')
                    byte |= (1 << bit_pos);
            }
            chararray[0] = byte;
            fwrite(chararray, 1, 1, output_file);
        }

        cout << "File is Successfully Compressed and Saved" << endl;
    }
    return 0;
}
