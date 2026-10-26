#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <ostream>
#include <string>
#include <vector>

enum class UserRole { Admin, Reader };//强类型枚举，防止污染命名空间

// 登录账户：管理员账户或读者账户
class Account {
public:
    Account(std::string username, std::string password, UserRole role,
            std::string readerId = "");

    const std::string& getUsername() const { return username_; }
    const std::string& getPassword() const { return password_; }
    UserRole getRole() const { return role_; }
    const std::string& getReaderId() const { return readerId_; }
    std::string getRoleName() const;
    bool isAdmin() const { return role_ == UserRole::Admin; }
    bool checkPassword(const std::string& password) const;
    void setPassword(const std::string& password) { password_ = password; }

    std::string serialize() const;

private:
    std::string username_;
    std::string password_;
    UserRole role_;
    std::string readerId_;
};

// 账户集合管理：登录校验、增删账户、修改/重置密码
class AccountManager {
public:
    static constexpr const char* kDefaultReaderPassword = "123456";
    static constexpr const char* kDefaultAdminUser = "admin";
    static constexpr const char* kDefaultAdminPassword = "admin123";

    AccountManager();

    // 没有任何管理员时创建默认管理员账户
    void ensureDefaults();

    Account* find(const std::string& username);
    const Account* find(const std::string& username) const;
    Account* authenticate(const std::string& username, const std::string& password);

    std::string add(const Account& account);
    bool remove(const std::string& username);
    bool changePassword(const std::string& username, const std::string& oldPassword,
                        const std::string& newPassword);
    bool resetPassword(const std::string& username, const std::string& newPassword);

    // 读者新增/删除时的账户联动
    std::string addReaderAccount(const std::string& readerId);
    void removeReaderAccount(const std::string& readerId);

    std::vector<Account>& all() { return accounts_; }
    const std::vector<Account>& all() const { return accounts_; }
    int adminCount() const;
    void clear() { accounts_.clear(); }

    void save(std::ostream& out) const;
    size_t loadLines(const std::vector<std::string>& lines);

private:
    std::vector<Account> accounts_;
};

#endif
