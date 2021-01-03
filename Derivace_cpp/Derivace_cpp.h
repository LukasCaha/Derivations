#pragma once

void ShuntingYard(std::vector<Token>& tokens, std::queue<Token>& output);

void ExtractTokens(std::string& line, std::vector<Token>& tokens);

void FillNode(Node& root, std::stack<Token>& output);
