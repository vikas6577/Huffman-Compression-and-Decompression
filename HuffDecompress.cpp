#include <bits/stdc++.h>
#define runfast                       \
    ios_base::sync_with_stdio(false); \
    cin.tie(NULL);                    \
    cout.tie(NULL)
using namespace std;

string decimal_to_binary(unsigned char n)
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

    while (ans.size() < 8)
        ans.push_back('0');
    reverse(ans.begin(), ans.end());
    return ans;
}
void set_file_name(string original, string &name)
{
    int n = original.size();
    for (int i = 0; i < n; i++)
    {
        if (original[i] == '-')
            break;
        name.push_back(original[i]);
    }
}

int main(int argc, char *argv[])
{
    runfast;

    // opening the input file using command line argument
    FILE *input_file = fopen(argv[1], "rb");
    if (input_file == NULL)
    {
        cout << "Input File Could Not be Opened" << endl;
        return 0;
    }
    else
    {
        string input_file_name = "";
        // setting the input file name
        set_file_name(argv[1], input_file_name);
        int inputnamesize=input_file_name.size();
        inputnamesize=inputnamesize-15;
        string inputfile=input_file_name.substr(0,inputnamesize);

        // this will be the name of the decompressed file
        string output_argument = inputfile + "_decompressed.";

        // finding the file type using the starting input of the input file
        int input_file_name_size = 0;
        char chararray[1];
        // read the size
        fread(chararray, 1, 1, input_file);
        input_file_name_size = chararray[0] - '0';
        for (int i = 0; i < input_file_name_size; i++)
        {
            fread(chararray, 1, 1, input_file);
            output_argument.push_back(chararray[0]);
        }

        FILE *output_file = fopen(output_argument.c_str(), "w");
        if (output_file == NULL)
        {
            cout << "Output File can't be Opened" << endl;
            return 0;
        }

        //   In the starting of the file we have details of the input file, after that we have codes, now we will iterate on all the codes, when we find two null chracters we stop there
        unordered_map<string, char> decoding_hashmap;
        int null = 1;
        string codes_mapping_to_char = "";
        while (null < 2)
        {
            fread(chararray, 1, 1, input_file);
            if (chararray[0] == '\0')
            {
                if (codes_mapping_to_char.size() > 0)
                    decoding_hashmap[codes_mapping_to_char.substr(1)] = codes_mapping_to_char[0];
                codes_mapping_to_char = "";
                null++;
            }
            else
            {
                codes_mapping_to_char.push_back(chararray[0]);
                null = 0;
            }
        }

        // Reading the padding from the input file
        fread(chararray, 1, 1, input_file);
        int padding = chararray[0] - '0';
        fread(chararray, 1, 1, input_file);

        // Now the main file is readed after the padding
        string s = "";
        while (fread(chararray, 1, 1, input_file))
        {
            string num = decimal_to_binary(chararray[0]);
            s += num;
        }
        // we will remove the last characters(number equal to padding)
        while (padding> 0){
            s.pop_back();
            padding--;
        }

        string temp = "";
        for (int i = 0; i < s.size(); i++)
        {
            temp.push_back(s[i]);
            if (decoding_hashmap.find(temp) != decoding_hashmap.end())
            {
                fwrite(&decoding_hashmap[temp], 1, 1, output_file);
                temp = "";
            }
        }

        fclose(input_file);
        fclose(output_file);

        cout << "File is Successfully Decompressed and Saved" << endl;
    }
    return 0;
}
