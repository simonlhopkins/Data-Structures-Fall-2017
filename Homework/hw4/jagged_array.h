#ifndef jagged_array_h
#define jagged_array_h


/*
This class Jagged Array is like an array, which in each index holds pointers to other arrays. For 
example a Jagged Array of Size 5 holds 5 different arrays in it. What makes this class very useful
and different from a conventional 2D array is that each array in the Jagged Array can be of varrying
size. You can then access certain values, add items, and check the size of each bin. Another feature
of Jagged Array is that you can "pack" it up, making the previous array of arrays into a single array.
When you pack it it takes up less memory because there is only one pointer to the array, rather than
having pointers to other pointers.

*/

template <class T> class JaggedArray{

public:

	//basic class constructor with int
	JaggedArray(int n){
		create(n);
	}
	//class constructor with another Jagged Array
	JaggedArray(const JaggedArray &ja){
		copy(ja);
	}
	//class constructor with = operator
	JaggedArray& operator=(const JaggedArray& ja);

	//copy helper function
	void copy(const JaggedArray &ja);
	//getters
	int numElements() const;
	int numBins() const;
	bool isPacked() const;
	T& getElement(int binNum, int slotNum);
	int numElementsInBin(int binNum);
	

	//setters
	void addElement(int index, const T& value);
	void removeElement(int binNum, int slotNum);
	void clear();
	void pack();
	void unpack();
	void print();
	//class deconstructor
	~JaggedArray(){
		//if it is packed it deletes all of the values at the end of the first pointers
		if(!isPacked()){
			for(int i=0; i<num_bins; i++){
				delete [] unpacked_values[i];
			}
			
		}
		//deletes all of the other values allocated on the heap
		delete [] packed_values;
		packed_values=NULL;
		delete [] offsets;
		offsets=NULL;
		delete [] counts;
		counts=NULL;
		delete [] unpacked_values;
		unpacked_values=NULL;

	}
	
private:
	//private member variables along with the create method (which is private because only Jagged
	//Array would ever need to access it)
	bool is_packed;
	void create(int n);
	int num_elements;
	int num_bins;
	int* counts;
	T** unpacked_values;
	int* offsets;
	T* packed_values;

};

//create method to create a JA with an int
template <class T> void JaggedArray<T>::create(int n) {
	//defaults is_packed to false
	is_packed=false;
	num_elements=0;
	num_bins=n;
	//creates new array on heap of size n for the counts
	counts = new int[n];
	//and then sets all values to 0 at the begining
	for(int i=0; i<n; i++){
		counts[i]=0;
	}
	//sets unpacked values to an array of pointers of size N
	unpacked_values = new T *[n];
	//defaults all of the pointers on the heap at the ends on unpacked values to null
	for(int i=0; i<n;i++){
		unpacked_values[i]=NULL;
	}
	//these two pointers are useless when its unpacked so they are null
	offsets=NULL;
	packed_values=NULL;

}

//copy constructor which copies over different variables depending on if the array that you are
//copying is unpacked or packed
template <class T> void JaggedArray<T>::copy(const JaggedArray &ja) {
	//if it is packed it copies over all of the neccesary packed variables
	if(ja.isPacked()){
		is_packed=true;
		num_elements= ja.num_elements;
		num_bins= ja.num_bins;
		counts= NULL;
		unpacked_values=NULL;
		packed_values= new T [num_elements];
		for(int i=0; i<num_elements; i++){
			packed_values[i]= ja.packed_values[i];
		}
		offsets= new int[num_bins];
		for(int i=0; i<num_bins;i++){
			offsets[i]=ja.offsets[i];
		}
	}
	//if it is unpacked it copies over all of the neccesary packed variables
	else{
		is_packed=false;

		num_elements= ja.num_elements;
		num_bins= ja.num_bins;
		offsets=NULL;
		packed_values=NULL;
		counts = new int[num_bins];
		for(int i=0; i<num_bins; i++){
			counts[i]= ja.counts[i];
		}
		unpacked_values= new T*[num_bins];
		for(int i=0; i<num_bins; i++){
			unpacked_values[i]= new T [counts[i]];
			for(int j=0; j<counts[i]; j++){
				unpacked_values[i][j]= ja.unpacked_values[i][j];
			}
		}
	}
}

/*this method handles any time you set one Jagged Array equal to another Jagged Array. It first checks
if the array you are setting is the same as the current array, in which it just returns the value of
itself. If it is different it deleres all of its current contents, and then copies the JA you are
setting, making sure you are not just leaving values from the old array floating in the heap.
*/
template <class T> JaggedArray<T>& JaggedArray<T>::operator=(const JaggedArray<T>& ja) {
	if (this != &ja) {
		if(!isPacked()){
			for(int i=0; i<num_bins; i++){
				delete [] unpacked_values[i];
			}
			
		}
		delete [] unpacked_values;
		unpacked_values=NULL;
		delete [] packed_values;
		packed_values=NULL;
		delete [] offsets;
		offsets=NULL;
		delete [] counts;
		counts=NULL;
		
		(*this).copy(ja);
	}
	return *this;
}

//Simple getter methods.
template <class T> int JaggedArray<T>::numElements() const{
	return num_elements;
}
template <class T> int JaggedArray<T>::numBins() const{
	return num_bins;
}
template <class T> bool JaggedArray<T>::isPacked() const{
	return is_packed;
}

//Function that adds to index of Jagged Array the value
template <class T> void JaggedArray<T>::addElement(int index, const T& value){
	
	//you can't add to a packed array so it exits immediately if it is packed.
	if(isPacked()){
		std::cerr<<"ERROR: Cannot add items to a packed array"<<std::endl;
		exit(1);
	}
	//you can't add to a bin number that doesn't exist 
	if(index>num_bins){
		std::cerr<<"ERROR: Bin does not exsist"<<std::endl;
		exit(1);
	}
	//increments the number of values in the index you stated
	counts[index]+=1;
	//creates a temp pointer of the old size of counts before you increment it
	T* tempArr= new T[counts[index]];
	//copies over the old values to the tempArr
	for(int i=0; i< counts[index]-1; i++){
		tempArr[i] = unpacked_values[index][i];
	}
	//sets the final value equal to the new value that you passed.
	tempArr[counts[index]-1]=value;
	//deletes the old value from the heap.
	delete [] unpacked_values[index];
	//points the unpacked array at index to the temp array you just created
	unpacked_values[index]= tempArr;
	//increments the total number of elements
	num_elements+=1;

	
}

template <class T> void JaggedArray<T>::removeElement(int binNum, int elementNum){
	//can't remove an element that does not exsist
	if(elementNum>counts[binNum]){
		std::cerr<<"Element does not exist in bin "<<binNum<<std::endl;
		exit(1);
	}
	//cnnot modify a packed Jagged Array
	if(isPacked()){
		std::cerr<<"ERROR: Cannot remove elements from a packed Jagged Array"<<std::endl;
		exit(1);
	}
	//can't access a bin that does not exist
	if(binNum>num_bins){
		std::cerr<<"ERROR: Bin does not exsist"<<std::endl;
		exit(1);
	}
	//decrement the number of elements in the bin number
	counts[binNum]-=1;
	//makes a temperary Array of Pointers equal to the new count of the bin after you remove an element
	T* tempArr = new T[counts[binNum]];
	//is true if a value actually was removed
	bool varRemoved= false;
	for(int i=0; i<counts[binNum]+1; i++){
		//if the i= element number sets var removed to true
		if(i==elementNum){
			varRemoved=true;
			continue;
		}
		//if the variable was removed, set the new array equal to the i value -1.
		if(varRemoved){
			tempArr[i-1]=unpacked_values[binNum][i];
		}
		//if it hasn't been removed yet set the new array value equal to i.
		else{
			tempArr[i]=unpacked_values[binNum][i];
		}
	}
	//delere the old array at unpacked values bin number
	delete [] *(unpacked_values+binNum);
	//sets it equal to the temp array we made earlier that has the new values with one removed
	*(unpacked_values+binNum)= tempArr;
	//decrement num_elements
	num_elements-=1;

}

//this method gets the elemet in binNum at elementNum
template <class T> T& JaggedArray<T>::getElement(int binNum, int elementNum){
	//can't remove an element that does not exsist
	// if(elementNum>counts[binNum]){
	// 	std::cerr<<"Element does not exist in bin "<<binNum<<std::endl;
	// 	exit(1);
	// }
	//can't access a bin that does not exist
	if(binNum>num_bins){
		std::cerr<<"Bin does not exsist"<<std::endl;
		exit(1);
	}
	//if it is unpacked it returns from the unpacked values array
	if(!isPacked()){
		return unpacked_values[binNum][elementNum];
	}
	//if it is packed it returns from the packed values array
	else{
		return packed_values[offsets[binNum]+elementNum];
	}
}

//this function sets every element equal to NULL whether it is packed or unpacked
template <class T> void JaggedArray<T>::clear(){
	if(isPacked()){
		//deletes the memeory of packed values and sets the pointer equal to null
		delete [] packed_values;
		packed_values= NULL;

		num_elements=0;
		//changes the offsets to all be 0
		for(int i=0; i<num_bins; i++){
			offsets[i]=0;
		}
	}
	else{
		//deletes each value at the end of all the unpacked values and sets all the counts equal to 0
		for(int i=0; i<num_bins; i++){
			
			delete [] unpacked_values[i];
			unpacked_values[i]=NULL;
			counts[i]=0;
			//std::cout<<tempUnpackedValues[i]<<std::endl;
		}
		//makes all of the counts equal to 0
		num_elements=0;
	}
	

}

//convert the unpacked Jagged Array into a packed array
template <class T> void JaggedArray<T>::pack(){
	//can't pack if it is already packed
	if(isPacked()){
		std::cerr<<"ERROR: Cannot pack a JaggedArray that is already packed";
		exit(1);
	}
	//sets the offsets to a value on heap of length num_bins
	offsets = new int [num_bins];
	//auto sets first value to 0
	offsets[0]=0;
	//helper variable to keep track off the offsets
	int total=0;
	//loops through all of the counts and calculates the offsets from them
	for(int i=0; i<num_bins-1; i++){
		total+=counts[i];
		offsets[i+1]=total;
	}
	//creates new array on Heap of the size of the total number of elements
	packed_values= new T[num_elements];

	int counter=0;
	//loops through unpacked values and sets them equal to each packed value
	for(int i=0; i<num_bins; i++){
		for(int j=0; j<counts[i]; j++){
			packed_values[counter]= getElement(i, j);
			++counter;
		}
	}


	is_packed=true;
	//delete counts
	delete [] counts;
	counts=NULL;
	//loops through and deletes all of the pointers at the ends of unpacked values
	for(int i=0; i<num_bins; i++){
		delete [] unpacked_values[i];
	}
	//creat unpacked Values
	delete [] unpacked_values;
	unpacked_values= NULL;


}


//converts packed JAs to unpack JAs
template <class T> void JaggedArray<T>::unpack(){
	//can't unpack an array thats is already unpacked
	if(!isPacked()){
		std::cerr<<"ERROR: Cannot unpacked a JaggedArray that is already unpacked";
		exit(1);
	}

	//created new array on heap of size num_bins for counts and caluclares the value based
	//on the offsets
	counts = new int[num_bins];
	for(int i=0; i<num_bins-1; i++){
		counts[i]=offsets[i+1]-offsets[i];
	}
	counts[num_bins-1]= num_elements-offsets[num_bins-1];

	//creates new unpacked array of pointers of size num_bins and creates a new pointer each index
	//of unpackd values
	unpacked_values = new T*[num_bins];
	for(int i=0; i<num_bins;i++){
		unpacked_values[i]=new T[counts[i]];
	}
	//sets the array at each index of unpacked values by using my getelement helper function on the
	//still exsisting packed array (because I havn't deleted packed values or set is_packed to false)
	for(int i=0; i<num_bins; i++){
		for(int j=0; j<counts[i]; j++){
			unpacked_values[i][j]=getElement(i,j);
		}
	}
	//deletes all unneccisary packed values
	is_packed=false;
	delete [] packed_values;
	packed_values=NULL;
	delete [] offsets;
	offsets= NULL;
}


//prints out all of the variables depending of if the Jagged Array is packed or not
template <class T> void JaggedArray<T>::print(){
	
	std::cout<<std::endl;
	std::cout<<"IS_PACKED: "<<isPacked()<<std::endl;
	std::cout<<"NUM_BINS: "<<num_bins<<std::endl;
	std::cout<<"NUM_ELEMENTS: "<<num_elements<<std::endl;
	std::cout<<"COUNTS: ";
	if(isPacked()){
		std::cout<<"NULL"<<std::endl;
	}
	else{
		for(int i=0; i<num_bins; i++){
			std::cout<<counts[i]<<", ";
		}
		std::cout<<std::endl;
	}
	std::cout<<"OFFSETS: ";
	if(isPacked()){
		for(int i=0; i<num_bins; i++){
			std::cout<<offsets[i]<<", ";
		}
		std::cout<<std::endl;
	}else{
		std::cout<<"NULL"<<std::endl;
	}

	std::cout<<"UNPACKED VALUES: "<<std::endl;
	if(isPacked()){
		std::cout<<"NULL"<<std::endl;
	}
	else{
		for(int i=0; i<num_bins; i++){
			std::cout<<i<<": "<<std::endl;
			for(int j=0; j<counts[i]; j++){

				std::cout<<"   "<<j<<": "<<unpacked_values[i][j]<<std::endl;
			}
		}
	}
	std::cout<<"PACKED VALUES: ";
	if(isPacked()){
		for(int i=0; i< num_elements; i++){
			std::cout<<packed_values[i]<<", ";
		}
		std::cout<<std::endl;
	}
	else{
		std::cout<<"NULL"<<std::endl;
	}
}

//gets the number of elements in each bin depending if it is packed or unpacked
template <class T> int JaggedArray<T>::numElementsInBin(int binNum){
	//easy if it is unpacked bevause we have a count array
	if(!isPacked()){
		return counts[binNum];
	}else{
		//if it is the last element we take the total number of elements and subtract it from the
		//last element in offset
		if(binNum==(num_bins-1)){
			return numElements()-offsets[num_bins-1];
		}else{
			//if it is not the last one we just take the next offset minus the offset at the current 
			//bin_num
			return offsets[binNum+1]-offsets[binNum];
		}
	}
	
}






#endif