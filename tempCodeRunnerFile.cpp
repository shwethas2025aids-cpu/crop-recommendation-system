#include <iostream>
#include <unordered_map>
#include <vector>
#include <numeric>
#include <queue>
#include <algorithm>
#include <iomanip>
#include <limits>
using namespace std;

class Campaign {
public:
    int id;
    string title;
    string creator;
    string category;
    double targetAmount;
    string deadline;
    vector<double> donations;

    Campaign() {
        id = 0;
        targetAmount = 0.0;
    }

    Campaign(int cid, string t, string c, string cat, double target, string d) {
        id = cid;
        title = t;
        creator = c;
        category = cat;
        targetAmount = target;
        deadline = d;
    }

    double getTotalFunds() const {
        return accumulate(donations.begin(), donations.end(), 0.0);
    }

    double getProgressPercentage() const {
        if (targetAmount <= 0) return 0.0;
        return (getTotalFunds() / targetAmount) * 100.0;
    }

    string getStatus() const {
        double total = getTotalFunds();

        if (total == 0)
            return "Not Started";
        else if (total < targetAmount)
            return "In Progress";
        else if (total == targetAmount)
            return "Fully Funded";
        else
            return "Over Funded";
    }

    double getHighestDonation() const {
        if (donations.empty()) return 0.0;
        return *max_element(donations.begin(), donations.end());
    }

    double getAverageDonation() const {
        if (donations.empty()) return 0.0;
        return getTotalFunds() / donations.size();
    }

    void display() const {
        cout << "\n----------------------------------------\n";
        cout << "Campaign ID      : " << id << "\n";
        cout << "Title            : " << title << "\n";
        cout << "Creator          : " << creator << "\n";
        cout << "Category         : " << category << "\n";
        cout << "Target Amount    : " << fixed << setprecision(2) << targetAmount << "\n";
        cout << "Deadline         : " << deadline << "\n";
        cout << "Total Funds      : " << getTotalFunds() << "\n";
        cout << "Progress         : " << getProgressPercentage() << "%\n";
        cout << "Status           : " << getStatus() << "\n";
        cout << "Highest Donation : " << getHighestDonation() << "\n";
        cout << "Average Donation : " << getAverageDonation() << "\n";
        cout << "Donation Count   : " << donations.size() << "\n";
    }
};

class CrowdfundingPlatform {
private:
    unordered_map<int, Campaign> campaigns;

public:
    void createCampaign() {
        int id;
        string title, creator, category, deadline;
        double targetAmount;

        cout << "Enter Campaign ID: ";
        cin >> id;

        if (campaigns.find(id) != campaigns.end()) {
            cout << "Campaign ID already exists.\n";
            return;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter Title: ";
        getline(cin, title);

        cout << "Enter Creator Name: ";
        getline(cin, creator);

        cout << "Enter Category: ";
        getline(cin, category);

        cout << "Enter Target Amount: ";
        cin >> targetAmount;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter Deadline: ";
        getline(cin, deadline);

        campaigns[id] = Campaign(id, title, creator, category, targetAmount, deadline);
        cout << "Campaign created successfully.\n";
    }

    void addDonation() {
        int id;
        double amount;

        cout << "Enter Campaign ID: ";
        cin >> id;

        auto it = campaigns.find(id);
        if (it == campaigns.end()) {
            cout << "Campaign not found.\n";
            return;
        }

        cout << "Enter Donation Amount: ";
        cin >> amount;

        if (amount <= 0) {
            cout << "Invalid donation amount.\n";
            return;
        }

        it->second.donations.push_back(amount);
        cout << "Donation added successfully.\n";
    }

    void viewCampaigns() const {
        if (campaigns.empty()) {
            cout << "No campaigns available.\n";
            return;
        }

        for (const auto &entry : campaigns) {
            entry.second.display();
        }
    }

    void trackFundingProgress() const {
        int id;
        cout << "Enter Campaign ID: ";
        cin >> id;

        auto it = campaigns.find(id);
        if (it == campaigns.end()) {
            cout << "Campaign not found.\n";
            return;
        }

        const Campaign &c = it->second;
        cout << "\nFunding Progress Report\n";
        cout << "Title         : " << c.title << "\n";
        cout << "Target Amount : " << c.targetAmount << "\n";
        cout << "Raised Amount : " << c.getTotalFunds() << "\n";
        cout << "Progress      : " << fixed << setprecision(2)
             << c.getProgressPercentage() << "%\n";
        cout << "Status        : " << c.getStatus() << "\n";
    }

    void rankCampaigns() const {
        if (campaigns.empty()) {
            cout << "No campaigns available.\n";
            return;
        }

        priority_queue<pair<double, int>> pq;

        for (const auto &entry : campaigns) {
            pq.push({entry.second.getTotalFunds(), entry.first});
        }

        cout << "\nCampaign Ranking by Funds Raised\n";
        int rank = 1;
        while (!pq.empty()) {
            double funds = pq.top().first;
            int id = pq.top().second;
            pq.pop();

            const Campaign &c = campaigns.at(id);
            cout << rank << ". "
                 << c.title
                 << " | Raised: " << fixed << setprecision(2) << funds
                 << " | Status: " << c.getStatus() << "\n";
            rank++;
        }
    }

    void updateCampaign() {
        int id;
        cout << "Enter Campaign ID to update: ";
        cin >> id;

        auto it = campaigns.find(id);
        if (it == campaigns.end()) {
            cout << "Campaign not found.\n";
            return;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter New Title: ";
        getline(cin, it->second.title);

        cout << "Enter New Creator Name: ";
        getline(cin, it->second.creator);

        cout << "Enter New Category: ";
        getline(cin, it->second.category);

        cout << "Enter New Target Amount: ";
        cin >> it->second.targetAmount;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter New Deadline: ";
        getline(cin, it->second.deadline);

        cout << "Campaign updated successfully.\n";
    }

    void generateReports() const {
        if (campaigns.empty()) {
            cout << "No campaigns available.\n";
            return;
        }

        double platformTotal = 0.0;
        int fundedCount = 0;

        auto maxCampaign = campaigns.begin();
        auto minCampaign = campaigns.begin();

        for (auto it = campaigns.begin(); it != campaigns.end(); ++it) {
            double total = it->second.getTotalFunds();
            platformTotal += total;

            if (total > maxCampaign->second.getTotalFunds()) {
                maxCampaign = it;
            }

            if (total < minCampaign->second.getTotalFunds()) {
                minCampaign = it;
            }

            if (total >= it->second.targetAmount && it->second.targetAmount > 0) {
                fundedCount++;
            }
        }

        cout << "\n========== PLATFORM REPORT ==========\n";
        cout << "Total Campaigns         : " << campaigns.size() << "\n";
        cout << "Total Funds Raised      : " << fixed << setprecision(2) << platformTotal << "\n";
        cout << "Funded Campaigns Count  : " << fundedCount << "\n";
        cout << "Highest Funded Campaign : "
             << maxCampaign->second.title << " ("
             << maxCampaign->second.getTotalFunds() << ")\n";
        cout << "Lowest Funded Campaign  : "
             << minCampaign->second.title << " ("
             << minCampaign->second.getTotalFunds() << ")\n";
        cout << "=====================================\n";
    }

    void searchByCategory() const {
        if (campaigns.empty()) {
            cout << "No campaigns available.\n";
            return;
        }

        string category;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter Category to Search: ";
        getline(cin, category);

        bool found = false;
        for (const auto &entry : campaigns) {
            if (entry.second.category == category) {
                entry.second.display();
                found = true;
            }
        }

        if (!found) {
            cout << "No campaigns found in this category.\n";
        }
    }
};

int main() {
    CrowdfundingPlatform platform;
    int choice;

    do {
        cout << "\n========== Crowdfunding Platform ==========\n";
        cout << "1. Create Campaign\n";
        cout << "2. Add Donation\n";
        cout << "3. View Campaigns\n";
        cout << "4. Track Funding Progress\n";
        cout << "5. Rank Campaigns\n";
        cout << "6. Update Campaign\n";
        cout << "7. Generate Reports\n";
        cout << "8. Search Campaign by Category\n";
        cout << "9. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                platform.createCampaign();
                break;
            case 2:
                platform.addDonation();
                break;
            case 3:
                platform.viewCampaigns();
                break;
            case 4:
                platform.trackFundingProgress();
                break;
            case 5:
                platform.rankCampaigns();
                break;
            case 6:
                platform.updateCampaign();
                break;
            case 7:
                platform.generateReports();
                break;
            case 8:
                platform.searchByCategory();
                break;
            case 9:
                cout << "Exiting program...\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }

    } while (choice != 9);

    return 0;
}