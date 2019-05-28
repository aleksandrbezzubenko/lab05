#include "Account.h"
#include "Transaction.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

class AMock : public Account {
public:
    AMock(int id, int balance) : Account(id, balance) {}
    MOCK_CONST_METHOD0(GetBalance, int());
    MOCK_METHOD1(ChangeBalance, void(int diff));
    MOCK_METHOD0(Lock, void());
    MOCK_METHOD0(Unlock, void());
};
class TMock : public Transaction {
public:
    MOCK_METHOD3(Make, bool(Account& from, Account& to, int sum));
};

TEST(Account, Mock) {
    AMock account(1, 200);
    EXPECT_CALL(account, GetBalance()).Times(1);
    EXPECT_CALL(account, ChangeBalance(testing::_)).Times(2);
    EXPECT_CALL(account, Lock()).Times(2);
    EXPECT_CALL(account, Unlock()).Times(1);
    account.GetBalance();
    account.ChangeBalance(100);
    account.Lock();
    account.ChangeBalance(100);
    account.Lock();
    account.Unlock();
}

TEST(Account, Test) {
    Account account(1, 200);
    EXPECT_EQ(account.GetBalance(), 200);

    account.Lock();
    EXPECT_THROW(account.ChangeBalance(-50), std::runtime_error);
    account.Unlock();
    EXPECT_EQ(account.GetBalance(), 150);
}

TEST(Transaction, Mock) {
    TransactionMock tr;
    Account account1(1, 50);
    Account account2(2, 500);
    EXPECT_CALL(tr, Make(testing::_, testing::_, testing::_))
        .Times(6);
    tr.set_fee(100);
    tr.Make(account1, account2, 199);
    tr.Make(account2, account1, 500);
    tr.Make(account2, account1, 300);
    tr.Make(account1, account1, 0);
    tr.Make(account1, account2, -1);
    tr.Make(account1, account2, 99);
}

TEST(Transaction, Test) {
    Account account1(1, 200);
    Account account2(2, 400);
    Transaction transaction;
    EXPECT_THROW(transaction.Make(account1, account1, 150), std::logic_error);
    EXPECT_THROW(transaction.Make(account1, account2, -15), std::invalid_argument);
    EXPECT_THROW(transaction.Make(account1, account2, 15), std::logic_error);
    transaction.set_fee(250);
    EXPECT_FALSE(transaction.Make(account1, account2, 100));
    transaction.set_fee(1);
    EXPECT_FALSE(transaction.Make(account1, account2, 200));
    EXPECT_FALSE(transaction.Make(account2, account1, 400));
    EXPECT_TRUE(transaction.Make(account1, account2, 100));
}
