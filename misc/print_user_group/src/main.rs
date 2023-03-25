use anyhow::bail;
use custom_debug_derive::Debug;
use std::fs::File;
use std::io::{self, BufRead};
use std::path::{Path, PathBuf};

#[derive(Debug)]
struct User {
    username: String,
    has_password: bool,
    user_id: u64,
    group_id: u64,
    user_id_info: String,
    home_directory: PathBuf,
    command: PathBuf,
}

impl User {
    fn parse_from_line(l: &str) -> anyhow::Result<User> {
        let v = l.split(':').collect::<Vec<_>>();
        if v.len() != 7 {
            bail!("Columns not equal to 7")
        }
        Ok(User {
            username: v[0].to_owned(),
            has_password: v[1].len() > 0,
            user_id: v[2].parse()?,
            group_id: v[3].parse()?,
            user_id_info: v[4].to_owned(),
            home_directory: Path::new(v[5]).to_path_buf(),
            command: Path::new(v[6]).to_path_buf(),
        })
    }
    fn parse_from_file(r: impl io::Read) -> anyhow::Result<Vec<User>> {
        let mut v = vec![];
        let lines = io::BufReader::new(r).lines();
        for res in lines {
            let l = res?;
            let u = User::parse_from_line(&l)?;
            v.push(u);
        }
        return Ok(v);
    }
}

#[derive(Debug)]
struct Group {
    group_name: String,
    #[debug(skip)]
    has_password: bool,
    group_id: u64,
    user_list: Vec<String>,
}

impl Group {
    fn parse_from_line(l: &str) -> anyhow::Result<Group> {
        let v = l.split(':').collect::<Vec<_>>();
        if v.len() != 4 {
            bail!("Columns not equal to 4")
        }
        Ok(Group {
            group_name: v[0].to_owned(),
            has_password: v[1].len() > 0,
            group_id: v[2].parse::<_>()?,
            user_list: v[3]
                .split(',')
                .filter(|s| s.len() > 0)
                .map(|s| s.to_owned())
                .collect::<_>(),
        })
    }
    fn parse_from_file(r: impl io::Read) -> anyhow::Result<Vec<Group>> {
        let mut v = vec![];
        let lines = io::BufReader::new(r).lines();
        for res in lines {
            let l = res?;
            let u = Group::parse_from_line(&l)?;
            v.push(u);
        }
        return Ok(v);
    }
}

fn main() -> anyhow::Result<()> {
    let user_accounts = {
        let filepath = "/etc/passwd";
        let file = File::open(filepath).unwrap();
        User::parse_from_file(file)
    };
    let groups = {
        let filepath = "/etc/group";
        let file = File::open(filepath).unwrap();
        Group::parse_from_file(file)?
    };
    for g in groups {
        println!("{:?}", g);
    }
    Ok(())
}
