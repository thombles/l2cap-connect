use libc::{AF_BLUETOOTH, SOCK_SEQPACKET, sa_family_t, c_ushort, sockaddr};

use std::error::Error;
use std::env;

const BTPROTO_L2CAP: i32 = 0;
const BDADDR_BREDR: u8 = 0x00;
const BDADDR_LE_PUBLIC: u8 = 0x01;
const BDADDR_LE_RANDOM: u8 = 0x02;

#[repr(packed)]
#[derive(Clone, Debug)]
struct bdaddr_t {
    b: [u8; 6],
}

#[repr(C)]
#[derive(Clone, Debug)]
struct sockaddr_l2 {
	l2_family: sa_family_t,
	l2_psm: c_ushort,
	l2_bdaddr: bdaddr_t,
	l2_cid: c_ushort,
	l2_bdaddr_type: u8,
}

fn main() -> Result<(), Box<dyn Error>> {
    println!("socket({}, {}, {})", AF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);
    let s = match unsafe {
        libc::socket(
            AF_BLUETOOTH,
            SOCK_SEQPACKET,
            BTPROTO_L2CAP,
        )
    } {
        i if i < 0 => return Err(Box::new(std::io::Error::last_os_error())),
        fd => fd
    };
    println!("Socket bound, fd {}", s);

    let addr_str = env::args().skip(1).next().expect("Must provide a connect address parameter");
    let addr: Vec<u8> = addr_str.split(":").map(|hex| u8::from_str_radix(hex, 16).unwrap()).collect();
    let bdaddr = bdaddr_t {
        b: [addr[5], addr[4], addr[3], addr[2], addr[1], addr[0]],
    };

    let psm: u16 = env::args().skip(2).next().unwrap().parse().unwrap();

    let sockaddr = sockaddr_l2 {
        l2_family: (AF_BLUETOOTH as u16).to_le(),
        l2_psm: psm.to_le(),
        l2_bdaddr: bdaddr,
        l2_cid: 0,
        l2_bdaddr_type: BDADDR_LE_RANDOM,
    };
    let sockaddr_sz = std::mem::size_of::<sockaddr_l2>();

    println!("Connecting to sockaddr_l2 {:?} (sz {}) on PSM {}", sockaddr, sockaddr_sz, psm);
    
    println!("connect({}, ... {})", s, sockaddr_sz);
    unsafe {
        println!("sockaddr: {:?}", std::mem::transmute::<sockaddr_l2, [u8; 14]>(sockaddr.clone()));
    }
    match unsafe {
        libc::connect(
            s,
            &sockaddr as *const sockaddr_l2 as *const sockaddr,
            sockaddr_sz as u32
        )
    } {
        i if i < 0 => return Err(Box::new(std::io::Error::last_os_error())),
        _ok => (),
    };
    println!("Connected");

    // TODO write message too

    Ok(())
}
