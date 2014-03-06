EXEC = xsq-convert

CC = g++
CFLAGS = -Wall -Wextra -O3 -march=native
BOOST_LIBS= -lboost_filesystem -lboost_system -lboost_program_options
HDF5_LIBS = -lhdf5_hl_cpp -lhdf5_cpp -lhdf5
LDFLAGS = $(BOOST_LIBS) $(HDF5_LIBS)

SRC_DIR = src
BUILD_DIR = build
SRC = $(wildcard $(SRC_DIR)/*.cc)
OBJECTS = $(patsubst $(SRC_DIR)/%.cc, $(BUILD_DIR)/%.o, $(SRC))

.PHONY : clean

all: $(BUILD_DIR) $(EXEC) 

$(EXEC): $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cc
	$(CC) $(CFLAGS) -c $< -o $@
	
$(BUILD_DIR):
	mkdir -p $@

clean:
	rm -rf $(EXEC) $(BUILD_DIR)
