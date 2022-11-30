#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
  using namespace std;

  int main(int argc, char **argv) { 

	      int cases = 0;
	          srand (time(0));
		      vector<int> keys;

		          string filename1 = "bigBoy.txt";
			      std::ofstream output_file1(filename1);

			          output_file1 << "N 4";

				      while(cases < 1000) {
					              string comand;
						              int key = rand()%(400000);
							              int dec = rand()%3;

								              if(dec == 1) {
										                  int dec1 = rand()%2;
												              if(dec1 == 1) {
														                      key = keys.at((rand()%keys.size()-1));
																                  }
													                  comand = "L " + to_string(key);
															          } else if (dec == 2){
																	              int dec1 = rand()%2;
																		                  if(dec1 == 1) {
																					                  key = keys.at((rand()%keys.size()-1));
																							              } else {
																									                  keys.push_back(key);
																											              }
																				              comand = "D " + to_string(key);
																					              } else {
																							                  string ker = to_string(key);
																									              keys.push_back(key);
																										                  comand = "I " + ker + " whoop whoop " + ker;
																												          }
									              output_file1 << "\n" << comand;

										          cases++;

				      }
				      output_file1.close();
  }
