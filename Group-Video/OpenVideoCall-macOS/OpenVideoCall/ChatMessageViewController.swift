//
//  ChatMessageViewController.swift
//  OpenVideoCall
//
//  Created by GongYuhua on 16/8/15.
//  Copyright © 2016年 Agora. All rights reserved.
//

import Cocoa

class ChatMessageViewController: NSViewController {
    
    @IBOutlet weak var messageTableView: NSTableView!
    
    fileprivate var messageList = [Message]()
    
    func append(chat text: String, fromUid uid: Int64) {
        let message = Message(text: text, type: .chat)
        append(message: message)
    }
    
    func append(alert text: String) {
        let message = Message(text: text, type: .alert)
        append(message: message)
    }
}

private extension ChatMessageViewController {
    func append(message: Message) {
        messageList.append(message)
        
        var deleted: Message?
        if messageList.count > 20 {
            deleted = messageList.removeFirst()
        }
        
        updateMessageTable(withDeleted: deleted)
    }
    
    func updateMessageTable(withDeleted deleted: Message?) {
        guard let tableView = messageTableView else {
            return
        }
        
        if deleted != nil {
            tableView.removeRows(at: IndexSet(integer: 0), withAnimation: .effectFade)
        }
        
        let lastRow = messageList.count - 1
        tableView.insertRows(at: IndexSet(integer: lastRow), withAnimation: .effectFade)
        tableView.scrollRowToVisible(lastRow)
    }
}

extension ChatMessageViewController: NSTableViewDataSource {
    func numberOfRows(in tableView: NSTableView) -> Int {
        return messageList.count
    }
}

extension ChatMessageViewController: NSTableViewDelegate {
    func tableView(_ tableView: NSTableView, viewFor tableColumn: NSTableColumn?, row: Int) -> NSView? {
        let cell = tableView.makeView(withIdentifier: NSUserInterfaceItemIdentifier(rawValue: "messageCell"), owner: self) as! ChatMessageCellView
        let message = messageList[row]
        cell.set(with: message)
        return cell
    }
    
    func tableView(_ tableView: NSTableView, heightOfRow row: Int) -> CGFloat {
        let defaultHeight: CGFloat = 24
        let string: NSString = messageList[row].text as NSString
        
        let column = tableView.tableColumns.first!
        let width = column.width - 24
        let textRect = string.boundingRect(with: NSMakeSize(width, 0),
                                           options: [.usesLineFragmentOrigin],
                                           attributes: [NSAttributedString.Key.font: NSFont.systemFont(ofSize: 12)])
        
        var textHeight = textRect.height + 6
        
        if textHeight < defaultHeight {
            textHeight = defaultHeight;
        }
        return textHeight;
    }
}
