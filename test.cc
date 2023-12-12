//#include "InttEvent.cc"

using namespace std;

int test(){
  
    string str = "/sphenix/tg/tg01/commissioning/INTT/merged_root_files/beam_inttall-00020867-0000_event_base_ana.root";

    char* cstr;
    cstr = new char[str.length()];
    strcpy(cstr, str.c_str());
    vector<string> cdata;

    char *p = strtok(cstr, "/_-.");
    while (p != NULL) {
        //std::cout << p << '\n';
        string str(p);
        cdata.push_back(p);
        p = strtok(NULL, "/_-.");
    }

    cout << cdata[10] << '\n';

    return 0;
}