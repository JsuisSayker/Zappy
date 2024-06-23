/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** test_hud
*/

#include "../include/ZappyGui.hpp" // Include the header file where ZappyGui::processCommand() is declared
#include <gtest/gtest.h>

// Mock or Stub the necessary classes if needed

// Define a fixture for repeated setup
class ZappyGuiTest : public ::testing::Test {
  protected:
    void SetUp() override
    {
        // Perform any necessary setup for each test
        // For example, initializing client, mutex, and setting up commands
        client = std::make_shared<zappy::Client>(); // Example, adjust as per your
                                             // implementation
        gui.setClient(client); // Set client for the ZappyGui object
    }

    void TearDown() override
    {
        // Clean up after each test if necessary
    }

    // Define member variables that might be needed across tests
    // Example:
    std::shared_ptr<zappy::Client> client;
    ZappyGui gui;
};

// Test case to verify the processCommand function
TEST_F(ZappyGuiTest, ProcessCommandTest)
{
    // Prepare test data and setup commands in the client's queue
    client.get()->pushToQueue("valid_command1 arg1 arg2");
    client.get()->pushToQueue("invalid_command");
    client.get()->pushToQueue("valid_command2 arg1 arg2");

    // Call the function you want to test
    gui.processCommand();

    // Add assertions to verify the expected behavior
    // Here, you might check the outcome of processing each command
    // For example, check if valid commands were executed correctly
    // and if unknown commands were handled properly.
    // Example assertions:
    // Check if valid_command1 was processed correctly
    EXPECT_TRUE(/* Add assertion here */);
    // Check if invalid_command produced the expected error message
    EXPECT_TRUE(/* Add assertion here */);
    // Check if valid_command2 was processed correctly
    EXPECT_TRUE(/* Add assertion here */);

    // You can add more specific assertions based on what processCommand()
    // should do
}

// Add more test cases as needed to cover different scenarios of
// processCommand()

// Entry point for running tests
int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
