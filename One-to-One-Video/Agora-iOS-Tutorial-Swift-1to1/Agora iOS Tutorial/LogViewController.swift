//
//  LogViewController.swift
//  Agora iOS Tutorial
//
//  Created by CavanSu on 2019/8/1.
//  Copyright © 2019 Agora.io. All rights reserved.
//

import UIKit

struct Log {
    enum ContentType {
        case info, warning, error
        
        var color: UIColor {
            switch self {
            case .info:     return UIColor(red: 74.0 / 255.0, green: 144.0 / 255.0, blue: 226.0 / 255.0, alpha: 1)
            case .warning:  return UIColor(red: 254.0 / 255.0, green: 221.0 / 255.0, blue: 86.0 / 255.0, alpha: 1)
            case .error:    return UIColor(red: 254.0 / 255.0, green: 55.0 / 255.0, blue: 95.0 / 255.0, alpha: 1)
            }
        }
    }
    
    var type: ContentType
    var content: String
}

class LogCell: UITableViewCell {
    @IBOutlet weak var content: UILabel!
    @IBOutlet weak var colorView: UIView!
    
    override func awakeFromNib() {
        super.awakeFromNib()
        colorView.layer.cornerRadius = 12.25
    }
    
    func update(_ log: Log) {
        content.text = log.content
        colorView.backgroundColor = log.type.color
    }
}

class LogViewController: UITableViewController {
    private lazy var list = [Log]()
    
    override func viewDidLoad() {
        super.viewDidLoad()
        tableView.rowHeight = UITableView.automaticDimension
        tableView.estimatedRowHeight = 44
    }
    
    override func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return list.count
    }
    
    override func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCell(withIdentifier: "LogCell", for: indexPath) as! LogCell
        cell.update(list[indexPath.row])
        return cell
    }
}

extension LogViewController {
    func log(type: Log.ContentType, content: String) {
        let log = Log(type: type, content: content)
        list.append(log)
        let index = IndexPath(row: list.count - 1, section: 0)
        tableView.insertRows(at: [index], with: .automatic)
        tableView.scrollToRow(at: index, at: .middle, animated: true)
    }
}
