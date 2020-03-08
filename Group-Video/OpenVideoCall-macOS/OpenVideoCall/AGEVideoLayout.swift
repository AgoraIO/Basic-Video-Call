//
//  AGEVideoLayout.swift
//  AGE
//
//  Created by CavanSu on 2019/7/23.
//  Copyright © 2019 CavanSu. All rights reserved.
//

#if os(iOS)
import UIKit
#else
import Cocoa
#endif

struct AGEIndex {
    var level: Int
    var item: Int
}

struct AGERankRow {
    var ranks: Int
    var rows: Int
}

struct AGEVideoLayout {
    enum Direction {
        case vertical, horizontal
    }
    
    enum LayoutType {
        case view
        #if os(iOS)
        case scroll(Direction)
        #endif
        
        var isScroll: Bool {
            switch self {
            #if os(iOS)
            case .scroll: return true
            #endif
            case .view:   return false
            }
        }
        
        fileprivate var rawValue: Int {
            switch self {
            case .view:
                return 0
            #if os(iOS)
            case .scroll(let direction):
                switch direction {
                case .horizontal: return 1
                case .vertical:   return 2
                }
            #endif
            }
        }
    }
    
    var interitemSpacing: CGFloat
    var lineSpacing: CGFloat
    
    var startPoint: CGPoint
    var itemSize: CGSize 
    var type: LayoutType
    var size: CGSize
    var level: Int
    
    init(interitemSpacing: CGFloat = 0,
         lineSpacing: CGFloat = 0,
         startPoint: CGPoint = CGPoint.zero,
         size: CGSize = CGSize(width: 1, height: 1),
         itemSize: CGSize = CGSize(width: 1, height: 1),
         type: LayoutType = .view,
         level: Int) {
        self.interitemSpacing = interitemSpacing
        self.lineSpacing = lineSpacing
        self.startPoint = startPoint
        self.itemSize = itemSize
        self.type = type
        self.size = size
        self.level = level
    }
    
    func type(_ type: LayoutType) -> AGEVideoLayout {
        var new = self
        new.type = type
        return new
    }
    
    func size(width: CGFloat? = nil, height: CGFloat? = nil) -> AGEVideoLayout {
        var new = self
        if let width = width {
            new.size.width = width
        }
        
        if let height = height {
            new.size.height = height
        }
        
        return new
    }
    
    func itemSize(width: CGFloat? = nil, height: CGFloat? = nil) -> AGEVideoLayout {
        var new = self
        if let width = width {
            new.itemSize.width = width
        }
        
        if let height = height {
            new.itemSize.height = height
        }
        
        return new
    }
    
    func startPoint(x: CGFloat? = nil, y: CGFloat? = nil) -> AGEVideoLayout {
        var new = self
        if let x = x {
            new.startPoint.x = x
        }
        
        if let y = y {
            new.startPoint.y = y
        }
        
        return new
    }
    
    func interitemSpacing(_ spacing: CGFloat) -> AGEVideoLayout {
        var new = self
        new.interitemSpacing = spacing
        return new
    }
    
    func lineSpacing(_ spacing: CGFloat) -> AGEVideoLayout {
        var new = self
        new.lineSpacing = spacing
        return new
    }
}

extension AGEVideoLayout {
    enum Equal {
        enum Reason {
            case minimumLineSpacing, minimumInteritemSpacing, startPoint, itemSize, size, layoutType
        }
        
        case yes, no([Reason])
        
        var boolValue: Bool {
            switch self {
            case .yes: return true
            case .no:  return false
            }
        }
    }
    
    static func ==(left: AGEVideoLayout, right: AGEVideoLayout) -> Bool {
        var isEqual = true
        
        if left.lineSpacing != right.lineSpacing {
            isEqual = false
            return isEqual
        }
        
        if left.interitemSpacing != right.interitemSpacing {
            isEqual = false
            return isEqual
        }
        
        if left.startPoint != right.startPoint {
            isEqual = false
            return isEqual
        }
        
        if left.itemSize != right.itemSize {
            isEqual = false
            return isEqual
        }
        
        if left.size != right.size {
            isEqual = false
            return isEqual
        }
        
        if left.type.rawValue != right.type.rawValue {
            isEqual = false
            return isEqual
        }
        
        return isEqual
    }
    
    func isEqual(right: AGEVideoLayout) -> AGEVideoLayout.Equal {
        
        var reasons = [AGEVideoLayout.Equal.Reason]()
        
        if self.lineSpacing != right.lineSpacing {
            reasons.append(.minimumLineSpacing)
        }
        
        if self.interitemSpacing != right.interitemSpacing {
            reasons.append(.minimumInteritemSpacing)
        }
        
        if self.startPoint != right.startPoint {
            reasons.append(.startPoint)
        }
        
        if self.itemSize != right.itemSize {
            reasons.append(.itemSize)
        }
        
        if self.size != right.size {
            reasons.append(.size)
        }
        
        if self.type.rawValue != right.type.rawValue {
            reasons.append(.layoutType)
        }
        
        var result: Equal = .yes
        if reasons.count > 0 {
            result = .no(reasons)
        }
        
        return result
    }
}

extension Array where Element == AGEVideoLayout.Equal.Reason {
    func containsOneOf(_ elements: [AGEVideoLayout.Equal.Reason]) -> Bool {
        var isContains = false
        
        for item in elements {
            if self.contains(item) {
                isContains = true
                break
            }
        }
        return isContains
    }
}
